#include "decimal.h"

decimal decimal_floor(decimal dec) {
  dec = truncate(dec);
  if (get_sign(dec)) {
    decimal one = {{0x1, 0x0, 0x0, 0x0}, 0x0};
    one = set_sign(one);
    dec = decimal_add(dec, one);
  }
  return dec;
}

decimal round(decimal dec) {
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
  dec = truncate(dec);
  if (mod > 5) {
    dec = decimal_add(dec, one);
  }
  if (mod == 5 && get_bit(dec, 0)) {
    dec = decimal_add(dec, one);
  }
  return dec;
}

decimal truncate(decimal dec) {
  int scale = get_scale(dec);
  while (scale > 0) {
    dec = ten_div(dec);
    set_scale(&dec, --scale);
  }
  return dec;
}

decimal negate(decimal x) {
  if (get_sign(x)) {
    x = rm_sign(x);
  } else {
    x = set_sign(x);
  }
  return x;
}
