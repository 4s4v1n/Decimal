#include "decimal.h"

int is_less(decimal x, decimal y) {
  int ret = FALSE;
  if (comparer(x, y) == -1) {
    ret = TRUE;
  }
  return ret;
}

int is_equal(decimal x, decimal y) {
  int ret = FALSE;
  if (comparer(x, y) == 0) {
    ret = TRUE;
  }
  return ret;
}

int is_greater(decimal x, decimal y) {
  int ret = FALSE;
  if (comparer(x, y) == 1) {
    ret = TRUE;
  }
  return ret;
}

int is_not_equal(decimal x, decimal y) { return !is_equal(x, y); }

int is_less_or_equal(decimal x, decimal y) {
  int ret = FALSE;
  if (comparer(x, y) == -1 || comparer(x, y) == 0) {
    ret = TRUE;
  }
  return ret;
}

int is_greater_or_equal(decimal x, decimal y) {
  int ret = FALSE;
  if (comparer(x, y) == 1 || comparer(x, y) == 0) {
    ret = TRUE;
  }
  return ret;
}

int comparer(decimal x, decimal y) {
  int res = -2;
  if (x.value_type == D_NORMAL_VALUE && y.value_type == D_NORMAL_VALUE) {
    int sign = 2 * get_sign(x) + get_sign(y);
    normalize(&x, &y);
    if (sign == 3) {
      res = -1 * first_different_bit(x, y);
    }
    if (sign == 2) {
      res = -1;
    }
    if (sign == 1) {
      res = 1;
    }
    if (sign == 0) {
      res = first_different_bit(x, y);
    }
  } else {
    if (x.value_type == D_NEGATIVE_INFINITY && y.value_type == INFINITY) {
      res = -1;
    }
    if (x.value_type == D_INFINITY && y.value_type == D_NEGATIVE_INFINITY) {
      res = 1;
    }
  }
  return res;
}
