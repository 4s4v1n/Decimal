#include "decimal.h"

void normalize(decimal* x, decimal* y) {
    int x_scale = get_scale(*x);
    int y_scale = get_scale(*y);
    while (x_scale > y_scale) {
        if (overflow_control(*y)) {
            ten_mult(y);
            y_scale++;
        } else {
            *x = normalize_rounding(*x);
            x_scale--;
        }
    }
    while (y_scale > x_scale) {
        if (overflow_control(*x)) {
            ten_mult(x);
            x_scale++;
        } else {
            *y = normalize_rounding(*y);
            y_scale--;
        }
    }
    set_scale(x, x_scale);
    set_scale(y, y_scale);
}

decimal normalize_rounding(decimal x) {
    int mod = 0;
    decimal one = {{0x1, 0x0, 0x0, 0x0}, D_NORMAL_VALUE};
    decimal ret = ten_div_mod_core(x, &mod);
    if (mod > 5) {
        ret = sum_wo_scale(ret, one);
    }
    if (mod == 5) {
        if (get_bit(ret, 0)) {
            ret = sum_wo_scale(ret, one);
        }
    }
    return ret;
}

int overflow_control(decimal dec) {
    int ret = FALSE;
    if ((unsigned int)dec.bits[HIGH] >= 0x1FFFFFFF) {
        ret = TRUE;
    }
    return ret;
}

int get_float_bit(float val, int i) {
    int mask = 1 << i;  // 10..0..0
    return !!(*((unsigned int*)&val) & mask);
}

int get_bin_exp(float val) {
    unsigned int bit = *((unsigned int*)&val);
    return (((bit & 0x7f800000) >> 23) - 127);
}

big_decimal integer_part(big_decimal x, big_decimal y, big_decimal* quotient);

decimal div_mod_core(decimal x, decimal y, decimal* mod) {
    decimal ret = {{0x0, 0x0, 0x0, 0x0}, 0x0};
    if (incorrect_values_div(x, y, &ret) == TRUE) {
        big_decimal big_x, big_y;
        convert_dec_to_big(x, &big_x);
        convert_dec_to_big(y, &big_y);
        int mod_scale = div_normalize(&big_x, &big_y);
        big_decimal quotient = {{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, 0x0};
        big_decimal tmp_dec = integer_part(big_x, big_y, &quotient);
        convert_big_to_dec(tmp_dec, mod);
        set_scale(mod, mod_scale);
        int big_scale = 0;
        while (first_big_bit(tmp_dec) != -1 && first_big_bit(quotient) < 98 && big_scale < 29) {
            while (first_different_big_bit(tmp_dec, big_y) < 0) {
                ten_big_mult(&tmp_dec);
                ten_big_mult(&quotient);
                big_scale++;
            }
            while (first_different_big_bit(tmp_dec, big_y) >= 0) {
                tmp_dec = sum_big_wo_scale(tmp_dec, big_dop_code(big_y));
                big_decimal one = {{0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, 0x0};
                quotient = sum_big_wo_scale(quotient, one);
            }
        }
        set_big_scale(&quotient, big_scale);
        convert_big_to_dec(quotient, &ret);
        if (get_sign(x) ^ get_sign(y)) {
            ret = set_sign(ret);
        }
    }
    return ret;
}
big_decimal integer_part(big_decimal x, big_decimal y, big_decimal* quotient) {
    big_decimal tmp_dec = {{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, 0x0};
    int i = first_big_bit(x);
    while (i >= 0) {
        left_shift_big_decimal(&tmp_dec, 1);
        if (get_big_bit(x, i--)) {
            tmp_dec = set_big_bit(tmp_dec, 0);
        }
        left_shift_big_decimal(quotient, 1);
        if (first_different_big_bit(tmp_dec, y) >= 0) {
            tmp_dec = sum_big_wo_scale(tmp_dec, big_dop_code(y));
            big_decimal one = {{0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, 0x0};
            *quotient = sum_big_wo_scale(*quotient, one);
        }
    }
    return tmp_dec;
}

int div_normalize(big_decimal* big_x, big_decimal* big_y) {
    int sub_scale = 0;
    while (get_big_scale(*big_x) && get_big_scale(*big_y)) {
        set_big_scale(big_x, get_big_scale(*big_x) - 1);
        set_big_scale(big_y, get_big_scale(*big_y) - 1);
        sub_scale++;
    }
    while (get_big_scale(*big_x) > 0) {
        set_big_scale(big_x, get_big_scale(*big_x) - 1);
        ten_big_mult(big_y);
        sub_scale++;
    }
    while (get_big_scale(*big_y) > 0) {
        set_big_scale(big_y, get_big_scale(*big_y) - 1);
        ten_big_mult(big_x);
        sub_scale++;
    }
    return sub_scale;
}

int incorrect_values_add(decimal x, decimal y, decimal* res) {
    int i = TRUE;
    if (x.value_type == D_NORMAL_VALUE && y.value_type == D_NORMAL_VALUE) {
        i = TRUE;
    } else if (x.value_type == D_INFINITY && y.value_type != D_NEGATIVE_INFINITY
                && y.value_type != D_NAN) {
        init_dec(res);
        res->value_type = D_INFINITY;
        i = FALSE;
    } else if (x.value_type == D_NEGATIVE_INFINITY && y.value_type != D_INFINITY
                && y.value_type != D_NAN) {
        init_dec(res);
        res->value_type = D_NEGATIVE_INFINITY;
        i = FALSE;
    } else if (y.value_type == D_INFINITY && x.value_type != D_NEGATIVE_INFINITY
                && x.value_type != D_NAN) {
        init_dec(res);
        res->value_type = D_INFINITY;
        i = FALSE;
    } else if (y.value_type == D_NEGATIVE_INFINITY && x.value_type != D_INFINITY
                && x.value_type != D_NAN) {
        init_dec(res);
        res->value_type = D_NEGATIVE_INFINITY;
        i = FALSE;
    } else {
        init_dec(res);
        res->value_type = D_NAN;
        i = FALSE;
    }
    return i;
}

int incorrect_values_div(decimal x, decimal y, decimal* res) {
    int i = TRUE;
    decimal zero = {{0x0, 0x0, 0x0, 0x0}, 0x0};
    if (x.value_type == D_NORMAL_VALUE && y.value_type == D_NORMAL_VALUE) {
        if (is_equal(x, zero) == TRUE && is_equal(y, zero) == TRUE) {
            i = FALSE;
            res->value_type = D_NAN;
        } else if (is_greater(x, zero) == TRUE && is_equal(y, zero) == TRUE) {
            i = FALSE;
            res->value_type = D_INFINITY;
        } else if (is_less(x, zero) == TRUE && is_equal(y, zero) == TRUE) {
            i = FALSE;
            res->value_type = D_NEGATIVE_INFINITY;
        }
    } else if (x.value_type == D_INFINITY && is_equal(y, zero) == TRUE) {
        i = FALSE;
        res->value_type = D_INFINITY;
    } else if (x.value_type == D_NEGATIVE_INFINITY && is_equal(y, zero) == TRUE) {
        i = FALSE;
        res->value_type = D_NEGATIVE_INFINITY;
    } else {
        i = FALSE;
        res->value_type = D_NAN;
    }
    return i;
}
