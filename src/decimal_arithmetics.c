#include "decimal.h"

decimal decimal_mul(decimal x, decimal y) {
  decimal res;
  if (x.value_type == D_NORMAL_VALUE && y.value_type == D_NORMAL_VALUE) {
    int sign = 0;  // если только у одного был знак
    if (get_sign(x) ^ get_sign(y)) sign = 1;
    big_decimal x_big, y_big, res_big;
    init_big_dec(&x_big);
    init_big_dec(&y_big);
    init_big_dec(&res_big);
    init_dec(&res);
    convert_dec_to_big(x, &x_big);
    convert_dec_to_big(y, &y_big);
    int x_scale = get_big_scale(x_big);
    int y_scale = get_big_scale(y_big);
    int res_scale = x_scale + y_scale;
    for (int i = 0; i < 192; i++) {
      if (i) {  // чтобы первый раз не смещать
        left_shift_big_decimal(&x_big, 1);
      }
      if (get_big_bit(y_big, i)) {
        res_big = sum_big_wo_scale(res_big, x_big);
      }
    }
    set_big_scale(&res_big, res_scale);
    convert_big_to_dec(res_big, &res);
    if (sign) {
      if (res.value_type == D_NORMAL_VALUE) {
        res = set_sign(res);
      } else {
        res.value_type = D_NEGATIVE_INFINITY;
      }
    }
  } else if (x.value_type == D_NAN || y.value_type == D_NAN) {
    res.value_type = D_NAN;
  } else if ((x.value_type == D_NEGATIVE_INFINITY) ^
             (y.value_type == D_NEGATIVE_INFINITY)) {
    res.value_type = D_NEGATIVE_INFINITY;
  } else {
    res.value_type = D_NEGATIVE_INFINITY;
  }
  return res;
}

decimal decimal_add(decimal x, decimal y) {
  decimal ret = {{0x0, 0x0, 0x0, 0x0}, 0x0};
  if (incorrect_values_add(x, y, &ret) == TRUE) {
    normalize(&x, &y);
    ret = sum_wo_scale(x, y);
    int scales = 2 * get_sign(x) + get_sign(y);
    if (scales == 3 && ret.value_type == D_NORMAL_VALUE) {
      ret = set_sign(ret);
    }
    if (scales == 2) {
      if (is_less_or_equal(dec_abs(x), dec_abs(y)) == TRUE) {
        ret = simple_subtraction(y, x);
        if (is_equal(dec_abs(x), dec_abs(y)) == TRUE) {
          ret = rm_sign(ret);
        }
      } else {
        ret = simple_subtraction(x, y);
        ret = set_sign(ret);
      }
    }
    if (scales == 1) {
      if (is_less_or_equal(dec_abs(x), dec_abs(y)) == TRUE) {
        ret = simple_subtraction(y, x);
        ret = set_sign(ret);
        if (is_equal(dec_abs(x), dec_abs(y)) == TRUE) {
          ret = rm_sign(ret);
        }
      } else {
        ret = simple_subtraction(x, y);
      }
    }
  }
  return ret;
}

decimal decimal_sub(decimal x, decimal y) {
  y = decimal_negate(y);
  return decimal_add(x, y);
}

decimal decimal_div(decimal x, decimal y) {
  decimal tmp;
  return div_mod_core(x, y, &tmp);
}

decimal decimal_mod(decimal x, decimal y) {
  decimal mod = {{0x0, 0x0, 0x0, 0x0}, 0x0};
  div_mod_core(x, y, &mod);
  if (get_sign(x)) {
    mod = set_sign(mod);
  }
  return mod;
}
