#include "decimal.h"

decimal decimal_floor(decimal dec) {
  dec = decimal_truncate(dec);
  if (get_sign(dec)) {
    decimal one = {{0x1, 0x0, 0x0, 0x0}, 0x0};
    one = set_sign(one);
    dec = decimal_add(dec, one);
  }
  return dec;
}

decimal decimal_round(decimal dec) {
  decimal one = {{0x1, 0x0, 0x0, 0x0}, 0x0};
  if (get_sign(dec)) {
    one = set_sign(one);
  }
  int scale = get_scale(dec);
  int mod = 0;
  while (scale >= 1) {
    dec = ten_div_mod_core(dec, &mod);
    set_scale(&dec, --scale);
  }
  dec = decimal_truncate(dec);
  if (mod > 5) {
    dec = decimal_add(dec, one);
  }
  if (mod == 5 && get_bit(dec, 0)) {
    dec = decimal_add(dec, one);
  }
  return dec;
}

decimal decimal_truncate(decimal dec) {
  int scale = get_scale(dec);
  while (scale > 0) {
    dec = ten_div(dec);
    set_scale(&dec, --scale);
  }
  return dec;
}

decimal decimal_negate(decimal dec) {
  if (get_sign(dec)) {
    dec = rm_sign(dec);
  } else {
    dec = set_sign(dec);
  }
  return dec;
}
