#include <check.h>
#include <stdio.h>
#include "../src/quickxor.h"

// core tests

START_TEST(creates) {
  QX* pqx = NULL;
  pqx     = QX_new();

  ck_assert_ptr_ne(pqx, NULL);

  QX_free(pqx);
}
END_TEST

START_TEST(has_correct_defaults) {
  QX* pqx = NULL;
  pqx     = QX_new();

  ck_assert_int_eq(pqx->kWidthInBits, 160);
  ck_assert_int_eq(pqx->kShift, 11);
  ck_assert_int_eq(pqx->kWidthInBytes, 20);
  ck_assert_int_eq(pqx->kDataLength, 3);
  ck_assert_int_eq(pqx->lengthSoFar, 0);
  ck_assert_int_eq(pqx->shiftSoFar, 0);
  ck_assert_ptr_ne(pqx->data, NULL);
  ck_assert_int_eq(*pqx->data, 0);

  QX_free(pqx);
}
END_TEST

// data tests

START_TEST(adds_data) {
  QX* pqx = NULL;
  pqx     = QX_new();

  uint8_t addData[] = {"abc"};

  QX_add(pqx, addData, 3);
  ck_assert_int_ne(*pqx->data, 0);
  ck_assert_int_eq(pqx->lengthSoFar, 3);
  ck_assert_int_eq(pqx->shiftSoFar, 33);

  QX_add(pqx, addData, 3);
  ck_assert_int_eq(pqx->lengthSoFar, 6);
  ck_assert_int_eq(pqx->shiftSoFar, 66);

  QX_free(pqx);
}
END_TEST

START_TEST(resets) {
  QX* pqx = NULL;
  pqx     = QX_new();

  uint8_t addData[] = {"abc"};
  QX_add(pqx, addData, 3);
  QX_reset(pqx);

  ck_assert_int_eq(*pqx->data, 0);
  ck_assert_int_eq(pqx->lengthSoFar, 0);
  ck_assert_int_eq(pqx->shiftSoFar, 0);

  QX_free(pqx);
}
END_TEST

START_TEST(calculates_digest) {
  QX* pqx = NULL;
  pqx     = QX_new();

  uint8_t addData[] = {"A short text"};
  QX_add(pqx, addData, 12);

  ck_assert_str_eq(QX_b64digest(pqx), "QQDBHNDwBjnQAQR0JAMe6AAAAAA=");
  // try again
  ck_assert_str_eq(QX_b64digest(pqx), "QQDBHNDwBjnQAQR0JAMe6AAAAAA=");

  QX_free(pqx);
}
END_TEST

// file tests

START_TEST(reads_file) {
  QX*  pqx = NULL;
  char filename[50];

  // illegal and inexistant values
  ck_assert_int_eq(QX_readFile(pqx, filename), 1);

  pqx = QX_new();
  strcpy(filename, "");
  ck_assert_int_eq(QX_readFile(pqx, filename), 1);

  pqx = QX_new();
  strcpy(filename, "iNeXiStAnT");
  ck_assert_int_eq(QX_readFile(pqx, filename), 1);

  // short_text.txt
  strcpy(filename, "short_text.txt");
  ck_assert_int_eq(QX_readFile(pqx, filename), 0);
  ck_assert_str_eq(QX_b64digest(pqx), "QQDBHNDwBjnQAQR0JAMe6AAAAAA=");

  // longer_text.txt
  strcpy(filename, "longer_text.txt");
  ck_assert_int_eq(QX_readFile(pqx, filename), 0);
  ck_assert_str_eq(QX_b64digest(pqx), "MyNPbFMLAm5Ol0JF4iqBwtfLtf8=");

  // perl_logo.svg
  strcpy(filename, "perl_logo.svg");
  ck_assert_int_eq(QX_readFile(pqx, filename), 0);
  ck_assert_str_eq(QX_b64digest(pqx), "t+ivKo9P9+OBdXUVle2LDwOmIzI=");

  // perl_camel.png
  strcpy(filename, "perl_camel.png");
  ck_assert_int_eq(QX_readFile(pqx, filename), 0);
  ck_assert_str_eq(QX_b64digest(pqx), "btGJtuvrt57YpSgEUpMJKkNQywA=");

  QX_free(pqx);
}
END_TEST

Suite* quickxorSuite() {
  Suite* suite    = NULL;
  TCase* tc_core  = NULL;
  TCase* tc_data  = NULL;
  TCase* tc_files = NULL;

  suite = suite_create("QuickXor");

  tc_core = tcase_create("Core");
  suite_add_tcase(suite, tc_core);
  tcase_add_test(tc_core, creates);
  tcase_add_test(tc_core, has_correct_defaults);

  tc_data = tcase_create("Data");
  suite_add_tcase(suite, tc_data);
  tcase_add_test(tc_core, adds_data);
  tcase_add_test(tc_core, resets);
  tcase_add_test(tc_core, calculates_digest);

  tc_files = tcase_create("Files");
  suite_add_tcase(suite, tc_files);
  tcase_add_test(tc_core, reads_file);

  return suite;
}

int main() {
  int      numberFailed;
  Suite*   suite;
  SRunner* runner;

  suite  = quickxorSuite();
  runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  numberFailed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return numberFailed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
