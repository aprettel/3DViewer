#include <check.h>
#include <time.h>

#include "viewer.h"
#define THRESHOLD 1E-5
#ifndef __S21_STDIO__
#define __S21_STDIO__
#include <stdio.h>
#endif
#include <math.h>

// #define SPEED_TEST

int eqVerts(struct Model *m1, struct Model *m2);
int eqSurf(struct Model *m1, struct Model *m2);

static double wtime();

START_TEST(base_load) {
  const double start = wtime();

  struct Model *base_model;
  const char *fileName = "3DViewer/TestModels/base.obj";
  base_model = loadModelFromFile(fileName);
  ck_assert(base_model != NULL);
  const double end = wtime();
  double result_time = 0;

  collapseModel(base_model);

#ifdef CLOCK_MONOTONIC
  result_time = end - start;
#else
  result_time = (end - start) / CLOCKS_PER_SEC;
#endif
  printf("Elapsed base_load tests time:%.8f seconds\n", result_time);
}
END_TEST

START_TEST(e_load) {
  const double start = wtime();

  struct Model *base_model;
  const char *fileName = "3DViewer/TestModels/pars.obj";
  base_model = loadModelFromFile(fileName);
  ck_assert(base_model != NULL);

  struct Model *ref_model;
  fileName = "3DViewer/TestModels/base.obj";
  ref_model = loadModelFromFile(fileName);
  ck_assert(ref_model != NULL);

  ck_assert(eqVerts(base_model, ref_model));
  ck_assert(eqSurf(base_model, ref_model));

  collapseModel(base_model);
  collapseModel(ref_model);

  const double end = wtime();
  double result_time = 0;

#ifdef CLOCK_MONOTONIC
  result_time = end - start;
#else
  result_time = (end - start) / CLOCKS_PER_SEC;
#endif
  printf("Elapsed e_load tests time:%.8f seconds\n", result_time);
}
END_TEST

START_TEST(rotation) {
  const double start = wtime();

  struct Model *base_model;
  const char *fileName = "3DViewer/TestModels/base.obj";
  base_model = loadModelFromFile(fileName);
  ck_assert(base_model != NULL);

  struct Model *ref_model;
  fileName = "3DViewer/TestModels/rotate.obj";
  ref_model = loadModelFromFile(fileName);
  ck_assert(ref_model != NULL);

  rotateModel(base_model, -35, 123, 200);
  ck_assert(eqVerts(base_model, ref_model));

  collapseModel(base_model);
  collapseModel(ref_model);

  const double end = wtime();
  double result_time = 0;

#ifdef CLOCK_MONOTONIC
  result_time = end - start;
#else
  result_time = (end - start) / CLOCKS_PER_SEC;
#endif
  printf("Elapsed rotation tests time:%.8f seconds\n", result_time);
}
END_TEST

START_TEST(move) {
  const double start = wtime();

  struct Model *base_model;
  const char *fileName = "3DViewer/TestModels/base.obj";
  base_model = loadModelFromFile(fileName);
  ck_assert(base_model != NULL);

  struct Model *ref_model;
  fileName = "3DViewer/TestModels/move.obj";
  ref_model = loadModelFromFile(fileName);
  ck_assert(ref_model != NULL);

  translateModel(base_model, 1, 3, -2);
  ck_assert(eqVerts(base_model, ref_model));

  collapseModel(base_model);
  collapseModel(ref_model);

  const double end = wtime();
  double result_time = 0;

#ifdef CLOCK_MONOTONIC
  result_time = end - start;
#else
  result_time = (end - start) / CLOCKS_PER_SEC;
#endif
  printf("Elapsed move tests time:%.8f seconds\n", result_time);
}
END_TEST

START_TEST(scale) {
  const double start = wtime();

  struct Model *base_model;
  const char *fileName = "3DViewer/TestModels/base.obj";
  base_model = loadModelFromFile(fileName);
  ck_assert(base_model != NULL);

  struct Model *ref_model;
  fileName = "3DViewer/TestModels/scale.obj";
  ref_model = loadModelFromFile(fileName);
  ck_assert(ref_model != NULL);

  scaleModel(base_model, 1.6988);
  ck_assert(eqVerts(base_model, ref_model));

  collapseModel(base_model);
  collapseModel(ref_model);

  const double end = wtime();
  double result_time = 0;

#ifdef CLOCK_MONOTONIC
  result_time = end - start;
#else
  result_time = (end - start) / CLOCKS_PER_SEC;
#endif
  printf("Elapsed scale tests time:%.8f seconds\n", result_time);
}
END_TEST

START_TEST(normalize) {
  const double start = wtime();

  struct Model *base_model;
  const char *fileName = "3DViewer/TestModels/scale.obj";
  base_model = loadModelFromFile(fileName);
  ck_assert(base_model != NULL);

  struct Model *ref_model;
  fileName = "3DViewer/TestModels/base.obj";
  ref_model = loadModelFromFile(fileName);
  ck_assert(ref_model != NULL);

  normalizeModel(base_model, -1, 1);
  ck_assert(eqVerts(base_model, ref_model));

  collapseModel(base_model);
  collapseModel(ref_model);

  const double end = wtime();
  double result_time = 0;

#ifdef CLOCK_MONOTONIC
  result_time = end - start;
#else
  result_time = (end - start) / CLOCKS_PER_SEC;
#endif
  printf("Elapsed normalize tests time:%.8f seconds\n", result_time);
}
END_TEST

int main() {
  const double runTime = 1000.0;
  Suite *suite = suite_create("calc test");
  TCase *test = tcase_create("calc test");
  SRunner *runner = srunner_create(suite);
  srunner_set_log(runner, "test_log.txt");
  suite_add_tcase(suite, test);
  tcase_set_timeout(test, runTime);

  tcase_add_test(test, base_load);
  tcase_add_test(test, rotation);
  tcase_add_test(test, move);
  tcase_add_test(test, scale);
  tcase_add_test(test, normalize);
  tcase_add_test(test, e_load);
  // tcase_add_test(test, rotation);
  // tcase_add_test(test, rotation);

  srunner_set_fork_status(runner, CK_FORK);
  srunner_run_all(runner, CK_ENV);
  int result = srunner_ntests_failed(runner);
  srunner_free(runner);
  return result;
}

static double wtime() {
  double result = 0.0;
#ifdef CLOCK_MONOTONIC
  struct timespec ts = {.tv_nsec = 0, .tv_sec = 0};
  clock_gettime(CLOCK_MONOTONIC, &ts);
  result = ts.tv_sec + ts.tv_nsec * 1E-9;
#else
  result = (double)clock();
#endif
  return result;
}

int eqVerts(struct Model *m1, struct Model *m2) {
  int eqFlag = 1;
  for (int i = 0; i < m1->numVertices; ++i) {
    double dx = fabs(m1->vertices[i].x - m2->vertices[i].x);
    double dy = fabs(m1->vertices[i].y - m2->vertices[i].y);
    double dz = fabs(m1->vertices[i].z - m2->vertices[i].z);
    eqFlag &= (dx < THRESHOLD) && (dy < THRESHOLD) && (dz < THRESHOLD);
    if (!eqFlag)
      printf("%d %lf %lf %lf\n", i, m1->vertices[i].x - m2->vertices[i].x,
             m1->vertices[i].y - m2->vertices[i].y,
             m1->vertices[i].z - m2->vertices[i].z);
  }
  return eqFlag;
}

int eqSurf(struct Model *m1, struct Model *m2) {
  int eqFlag = 1;
  for (int i = 0; i < m1->numSurfaces; ++i) {
    eqFlag &= (m1->surfaces[i].v1 == m2->surfaces[i].v1) &&
              (m1->surfaces[i].v2 == m2->surfaces[i].v2) &&
              (m1->surfaces[i].v3 == m2->surfaces[i].v3);
  }
  return eqFlag;
}