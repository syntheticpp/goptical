/*

      This file is part of the Goptical Core library.
  
      The Goptical library is free software; you can redistribute it
      and/or modify it under the terms of the GNU General Public
      License as published by the Free Software Foundation; either
      version 3 of the License, or (at your option) any later version.
  
      The Goptical library is distributed in the hope that it will be
      useful, but WITHOUT ANY WARRANTY; without even the implied
      warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
      See the GNU General Public License for more details.
  
      You should have received a copy of the GNU General Public
      License along with the Goptical library; if not, write to the
      Free Software Foundation, Inc., 59 Temple Place, Suite 330,
      Boston, MA 02111-1307 USA
  
      Copyright (C) 2011 Free Software Foundation, Inc
      Author: Alexandre Becoulet

*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlin.h>

#define FIRSTWL 390
#define LASTWL  750
#define STEPWL  1
#define NWL ((LASTWL - FIRSTWL) / STEPWL)

struct glass {
  double nd;
  double vd;
  double dpgf;
  double sl[6];     // sellmeier data
  double nwl[NWL];  // index at wavelen from sellmeier
};

struct glass *gcat[256];
size_t glass_cnt = 0;

//#define ABBE_USE_VENE

// dpgf normal line coefs from schott TIE-29: Refractive Index and Dispersion
double a0 = 0.6438;
double a1 = -0.001682;

static int
expb_f(const gsl_vector * x, void *data, gsl_vector * f)
{
  double c0 = gsl_vector_get (x, 0);
  double c1 = gsl_vector_get (x, 1);
  double c2 = gsl_vector_get (x, 2);
  double c3 = gsl_vector_get (x, 3);
  double c4 = gsl_vector_get (x, 4);
  double c5 = gsl_vector_get (x, 5);
  double c6 = gsl_vector_get (x, 6);
  double c7 = gsl_vector_get (x, 7);

  double cc = 0, max = 0;

  size_t i, j;
  for (i = 0; i < glass_cnt; i++)
    {
      struct glass *g = gcat[i];
      double vd = g->vd;
      double dpgf = g->dpgf;
      double b = (g->nd - 1.0) / g->vd;
      double a = (g->vd * a1) + a0 + g->dpgf;

      for (j = 0; j < NWL; j++)
	{
	  double wl = (FIRSTWL + j * STEPWL) / 1000.;
	  double w2 = wl * wl;
	  double w3 = wl * wl * wl;
	  double e1 = (a * c0 + c1) + (a * c2 + c3) / wl + (a * c4 + c5) / w2 + (a * c6 + c7) / w3 ;
	  double e2 = g->nd + b * e1;

	  gsl_vector_set(f, i * NWL + j, e2 - g->nwl[j]);

	  double err = fabs(e2 - g->nwl[j]);
	  cc += err;
	  if (err > max)
	    max = err;
	}
    }

  printf("mean err %.20lf, max err %.20lf\n", cc / glass_cnt / NWL, max);

  return GSL_SUCCESS;
}

int expb_df (const gsl_vector * x, void *data, gsl_matrix * J)
{
  double c0 = gsl_vector_get (x, 0);
  double c1 = gsl_vector_get (x, 1);
  double c2 = gsl_vector_get (x, 2);
  double c3 = gsl_vector_get (x, 3);
  double c4 = gsl_vector_get (x, 4);
  double c5 = gsl_vector_get (x, 5);
  double c6 = gsl_vector_get (x, 6);
  double c7 = gsl_vector_get (x, 7);

  size_t i, j;
  for (i = 0; i < glass_cnt; i++)
    {
      struct glass *g = gcat[i];
      double vd = g->vd;
      double nd = g->nd;
      double dpgf = g->dpgf;
      double q = (nd - 1.0) / vd;

      for (j = 0; j < NWL; j++)
	{
	  double wl = (FIRSTWL + j * STEPWL) / 1000.;
	  double w2 = wl * wl;
	  double w3 = wl * wl * wl;

	  gsl_matrix_set (J, i * NWL + j, 0, q * (a1*vd+dpgf+a0));
	  gsl_matrix_set (J, i * NWL + j, 1, q);
	  gsl_matrix_set (J, i * NWL + j, 2, q * (a1*vd+dpgf+a0)/wl);
	  gsl_matrix_set (J, i * NWL + j, 3, q / wl);
	  gsl_matrix_set (J, i * NWL + j, 4, q * (a1*vd+dpgf+a0)/w2);
	  gsl_matrix_set (J, i * NWL + j, 5, q / w2);
	  gsl_matrix_set (J, i * NWL + j, 6, q * (a1*vd+dpgf+a0)/w3);
	  gsl_matrix_set (J, i * NWL + j, 7, q / w3);
	}
    }

  return GSL_SUCCESS;
}

int
expb_fdf(const gsl_vector * x, void *data,
	  gsl_vector * f, gsl_matrix * J)
{
  expb_f(x, data, f);
  expb_df(x, data, J);

  return GSL_SUCCESS;
}

int
main (void)
{
  const gsl_multifit_fdfsolver_type *T;
  gsl_multifit_fdfsolver *s;
  int status;
  unsigned int i, j, iter = 0;
  const size_t p = 8;

  gsl_multifit_function_fdf f;

  double x_init[8] = {
    0.67815, -2.59048,
    6.00933, -2.12533,
    -7.30473, 3.88332,
    2.07977, -1.02247,
  };

  gsl_vector_view x = gsl_vector_view_array(x_init, p);

  char buf[256];
  FILE *file = fopen("schott_data.txt", "r");

  if (!file)
    exit(1);

  while (fgets(buf, sizeof(buf), file))
    {
      if (buf[0] == '#')
	continue;

      i = glass_cnt++;
      struct glass *g = gcat[i] = malloc(sizeof(struct glass));

      //               nm  vd  ve   B1  B2  B3  C1  C2  C3  nd  ne   DP g.F
#ifdef ABBE_USE_VENE
      if (sscanf(buf, "%*s %*lf %lf %lf %lf %lf %lf %lf %lf %*lf %lf %lf",
#else
      if (sscanf(buf, "%*s %lf %*lf %lf %lf %lf %lf %lf %lf %lf %*lf %lf",
#endif
		&g->vd, g->sl + 0, g->sl + 1, g->sl + 2, g->sl + 3, g->sl + 4, g->sl + 5, &g->nd, &g->dpgf) != 9)
	continue;

      // load glass data here

      for (j = 0; j < NWL; j++)
	{
	  double wl = (FIRSTWL + j * STEPWL) / 1000.;
	  double w2 = wl * wl;
	  double ndx = 1.0;

	  // sellmeier model
	  ndx += (w2 * g->sl[0]) / (w2 - g->sl[3]);
	  ndx += (w2 * g->sl[1]) / (w2 - g->sl[4]);
	  ndx += (w2 * g->sl[2]) / (w2 - g->sl[5]);

	  g->nwl[j] = sqrt(ndx);
	}
    }

  fclose(file);

  printf("%i glasses loaded\n", glass_cnt);

  const size_t n = NWL * glass_cnt;

  f.f = &expb_f;
  f.df = &expb_df;
  f.fdf = &expb_fdf;
  f.n = n;
  f.p = p;

  T = gsl_multifit_fdfsolver_lmsder;
  s = gsl_multifit_fdfsolver_alloc (T, n, p);
  gsl_multifit_fdfsolver_set (s, &f, &x.vector);

  do
    {
      iter++;
      status = gsl_multifit_fdfsolver_iterate(s);

      printf ("status = %s\n", gsl_strerror (status));

      if (status)
	break;

      status = gsl_multifit_test_delta (s->dx, s->x, 1e-6, 1e-6);
    }
  while (status == GSL_CONTINUE && iter < 20);

  gsl_matrix *covar = gsl_matrix_alloc (p, p);
  gsl_multifit_covar (s->J, 0.0, covar);

  {
    double chi = gsl_blas_dnrm2(s->f);
    double dof = n - p;
    double c = GSL_MAX_DBL(1, chi / sqrt(dof));

    printf("chisq/dof = %g\n",  pow(chi, 2.0) / dof);

    for (i = 0; i < p; i++)
      printf ("c%i = %.20f +/- %.5f\n", i,
	      gsl_vector_get(s->x, i),
	      sqrt(gsl_matrix_get(covar,i,i)));
  }

  printf ("status = %s\n", gsl_strerror (status));

  gsl_multifit_fdfsolver_free(s);
  gsl_matrix_free(covar);

  return 0;
}
