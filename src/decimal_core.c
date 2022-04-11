#include "decimal.h"

int get_bit(decimal val, int i) {
  int mask = 1 << (i % 32);  // получаем 0..010..0
  return !!(val.bits[i / 32] & mask);
}

decimal rm_bit(decimal val, int i) {
  int mask = ~(1 << (i % 32));  // получаем 1..101..1
  val.bits[i / 32] &= mask;
  return val;
}

decimal set_bit(decimal val, int i) {
  int mask = 1 << (i % 32);  // получаем 0..010..0
  val.bits[i / 32] |= mask;
  return val;
}

int get_sign(decimal val) {
  int mask = 1 << 31;  // получим 10..00
  val.bits[SCALE] &= mask;
  return !!val.bits[SCALE];
}

decimal set_sign(decimal val) {
  int mask = 1 << 31;  // получим 10..00
  val.bits[SCALE] |= mask;
  return val;
}

decimal rm_sign(decimal val) {
  int mask = ~(1 << 31);  // получим 01..11
  val.bits[SCALE] &= mask;
  return val;
}

int get_scale(decimal val) {
  val = rm_sign(val);
  return val.bits[SCALE] >> 16;
}

void set_scale(decimal* val, int scale) {
  rm_scale(val);
  scale = scale << 16;
  val->bits[SCALE] |= scale;
}

void rm_scale(decimal* val) {
  int mask = 1 << 31;
  val->bits[SCALE] &= mask;
}

void init_dec(decimal* dec) {
  dec->bits[LOW] = 0;
  dec->bits[MID] = 0;
  dec->bits[HIGH] = 0;
  dec->bits[SCALE] = 0;
  dec->value_type = D_NORMAL_VALUE;
}

void left_shift_decimal(decimal* val, int shift) {
  decimal res;
  init_dec(&res);
  for (int i = 0; i + shift < 96; i++) {
    if (get_bit(*val, i)) res = set_bit(res, i + shift);
  }
  *val = res;
}

void right_shift_decimal(decimal* val, int shift) {
  decimal res;
  init_dec(&res);
  for (int i = 95; i - shift >= 0; i--) {
    if (get_bit(*val, i)) res = set_bit(res, i - shift);
  }
  *val = res;
}

decimal dop_code(decimal dec) {
  dec.bits[LOW] = ~dec.bits[LOW];
  dec.bits[MID] = ~dec.bits[MID];
  dec.bits[HIGH] = ~dec.bits[HIGH];
  dec.value_type = D_DOP_CODE;
  decimal one = {{0x1, 0x0, 0x0, 0x0}, D_DOP_CODE};
  return sum_wo_scale(dec, one);
}

void ten_mult(decimal* val) {
  decimal buf_1, buf_3;
  buf_1 = *val;
  buf_3 = *val;
  left_shift_decimal(&buf_1, 1);  // исходное смещённое на 1
  left_shift_decimal(&buf_3, 3);  // исходное смещённое на 3
  *val = sum_wo_scale(buf_1, buf_3);
}

decimal sum_wo_scale(decimal x, decimal y) {
  int mind = 0;  //  единица в уме
  decimal res;
  init_dec(&res);
  for (int i = 0; i < 96; i++) {
    if (get_bit(x, i) && get_bit(y, i)) {
      if (mind) res = set_bit(res, i);
      mind = 1;
    } else if (!get_bit(x, i) && !get_bit(y, i)) {
      if (mind) {
        res = set_bit(res, i);
        mind = 0;
      }
    } else {
      if (mind) {
        mind = 1;
      } else {
        res = set_bit(res, i);
      }
    }
    set_scale(&res, get_scale(x));
  }
  if (mind) {
    if (y.value_type != D_DOP_CODE) {
      init_dec(&res);
      if (!get_sign(x)) {
        res.value_type = D_INFINITY;
      } else {
        res.value_type = D_NEGATIVE_INFINITY;
      }
    } else {
      res.value_type = D_NORMAL_VALUE;
    }
  }
  if (x.value_type == D_DOP_CODE) {
    res.value_type = D_DOP_CODE;
  }
  return res;
}

decimal ten_div(decimal dec) {
  int mod = 0;
  return ten_div_mod_core(dec, &mod);
}

decimal ten_div_mod_core(decimal dec, int* mod) {
  decimal tmp_dec = {{0x0, 0x0, 0x0, 0x0}, 0x0};
  decimal quotient = {{0x0, 0x0, 0x0, 0x0}, 0x0};
  for (int i = 95; i >= 0; i--) {
    left_shift_decimal(&tmp_dec, 1);
    if (get_bit(dec, i)) {
      tmp_dec = set_bit(tmp_dec, 0);
    }
    left_shift_decimal(&quotient, 1);
    if (tmp_dec.bits[LOW] >= 0xA) {
      tmp_dec.bits[LOW] -= 0xA;
      decimal one = {{0x1, 0x0, 0x0, 0x0}, 0x0};
      quotient = decimal_add(quotient, one);
    }
  }
  *mod = tmp_dec.bits[LOW];
  if (get_sign(dec)) {
    quotient = set_sign(quotient);
  }
  return quotient;
}

decimal simple_subtraction(decimal x, decimal y) {
  y = dop_code(y);
  return sum_wo_scale(x, y);
}

decimal dec_abs(decimal x) {
  x = rm_sign(x);
  return x;
}

int first_different_bit(decimal x, decimal y) {
  int ret = 0;
  for (int i = 95; i >= 0 && ret == 0; i--) {
    ret = get_bit(x, i) - get_bit(y, i);
  }
  return ret;
}
