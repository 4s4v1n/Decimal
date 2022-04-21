#include <check.h>
#include <limits.h>
#include <stdlib.h>

#include "decimal.h"

#define ck_assert_decimal_eq(test_dec, control_dec)                  \
    ck_assert_int_eq(test_dec.bits[LOW], control_dec.bits[LOW]);     \
    ck_assert_int_eq(test_dec.bits[MID], control_dec.bits[MID]);     \
    ck_assert_int_eq(test_dec.bits[HIGH], control_dec.bits[HIGH]);   \
    ck_assert_int_eq(test_dec.bits[SCALE], control_dec.bits[SCALE]); \
    ck_assert_int_eq(test_dec.value_type, control_dec.value_type);

START_TEST(get_bit_test) {
    decimal dec;
    init_dec(&dec);
    dec.bits[LOW] = 0xFFFFFFFF;
    dec.bits[MID] = 0xFFFFFFFF;
    dec.bits[HIGH] = 0xFFFFFFFF;
    dec.bits[SCALE] = 0xFF000000;
    ck_assert_int_eq(get_bit(dec, 0), 1);
    ck_assert_int_eq(get_bit(dec, 127), 1);
}
END_TEST

START_TEST(rm_bit_test) {
    decimal test_dec, control_dec;
    init_dec(&test_dec);
    init_dec(&control_dec);
    test_dec.bits[LOW] = 0x1;
    control_dec.bits[LOW] = 0x3;
    control_dec = rm_bit(control_dec, 1);
    ck_assert_decimal_eq(test_dec, control_dec);
}
END_TEST

START_TEST(set_bit_test) {
    decimal test_dec, control_dec;
    init_dec(&test_dec);
    init_dec(&control_dec);
    test_dec.bits[SCALE] = 0x80000000;
    control_dec = set_bit(control_dec, 127);
    ck_assert_decimal_eq(test_dec, control_dec);
}
END_TEST

START_TEST(set_sign_test) {
    decimal test_dec, control_dec;
    init_dec(&test_dec);
    init_dec(&control_dec);
    test_dec.bits[SCALE] = 0x80000000;
    control_dec = set_sign(control_dec);
    ck_assert_decimal_eq(test_dec, control_dec);
}
END_TEST

START_TEST(rm_sign_test) {
    decimal test_dec, control_dec;
    init_dec(&test_dec);
    init_dec(&control_dec);
    test_dec.bits[SCALE] = 0x80000000;
    test_dec = rm_sign(test_dec);
    ck_assert_decimal_eq(test_dec, control_dec);
}
END_TEST

START_TEST(get_sign_test) {
    decimal test_dec;
    init_dec(&test_dec);
    ck_assert_int_eq(get_sign(test_dec), 0);
}
END_TEST

START_TEST(get_scale_test) {
    decimal test_dec;
    init_dec(&test_dec);
    set_scale(&test_dec, 12);
    ck_assert_int_eq(get_scale(test_dec), 12);
}
END_TEST

START_TEST(set_scale_test) {
    decimal test_dec, control_dec;
    init_dec(&test_dec);
    init_dec(&control_dec);
    test_dec.bits[SCALE] = 12 << 12;
    set_scale(&test_dec, 0);
    ck_assert_decimal_eq(test_dec, control_dec);
}
END_TEST

START_TEST(rm_scale_test) {
    decimal test_dec, control_dec;
    init_dec(&test_dec);
    init_dec(&control_dec);
    set_scale(&test_dec, 15);
    rm_scale(&test_dec);
    ck_assert_decimal_eq(test_dec, control_dec);
}
END_TEST

START_TEST(left_shift_test) {
    decimal test_dec, control_dec;
    init_dec(&test_dec);
    init_dec(&control_dec);
    test_dec.bits[LOW] = 0x2;
    control_dec.bits[HIGH] = 0x80000000;
    left_shift_decimal(&test_dec, 94);
    ck_assert_decimal_eq(test_dec, control_dec);
}
END_TEST

START_TEST(right_shift_test) {
    decimal test_dec, control_dec;
    init_dec(&test_dec);
    init_dec(&control_dec);
    test_dec.bits[HIGH] = 0x80000000;
    control_dec.bits[LOW] = 0x1;
    right_shift_decimal(&test_dec, 95);
    ck_assert_decimal_eq(test_dec, control_dec);
}
END_TEST

START_TEST(template) {
    decimal test_dec;
    decimal control_dec = {0, 0, 0, 0x0, 0};
    init_dec(&test_dec);
    ck_assert_decimal_eq(test_dec, control_dec);
}
END_TEST

START_TEST(from_int_to_decimal_test) {
    decimal test_dec, control_dec;
    init_dec(&test_dec);
    init_dec(&control_dec);
    int test_int = INT_MAX;
    from_int_to_decimal(test_int, &test_dec);
    control_dec.bits[LOW] = INT_MAX;
    ck_assert_decimal_eq(test_dec, control_dec);

    test_int = -INT_MAX;
    control_dec.bits[LOW] = INT_MAX;
    control_dec = set_sign(control_dec);
    from_int_to_decimal(test_int, &test_dec);
    ck_assert_decimal_eq(test_dec, control_dec);

    init_dec(&test_dec);
    init_dec(&control_dec);
    test_int = 4;
    control_dec.bits[LOW] = 0x4;
    test_dec.bits[MID] = 0xF;
    from_int_to_decimal(test_int, &test_dec);
    ck_assert_decimal_eq(test_dec, control_dec);
}
END_TEST


START_TEST(from_decimal_to_int_1) {
    int test_int = 0;
    int control_int = INT_MAX;
    decimal test_dec;
    init_dec(&test_dec);
    test_dec.bits[LOW] = INT_MAX;
    from_decimal_to_int(test_dec, &test_int);
    ck_assert_int_eq(test_int, control_int);
}
END_TEST

START_TEST(from_decimal_to_int_2) {
    int test_int = 0;
    int control_int = 124124124;
    decimal test_dec;
    init_dec(&test_dec);
    test_dec.bits[LOW] = 124124124;
    from_decimal_to_int(test_dec, &test_int);
    ck_assert_int_eq(test_int, control_int);
}
END_TEST

START_TEST(from_decimal_to_int_3) {
    int test_int = 0;
    int control_int = -124124;
    decimal test_dec;
    init_dec(&test_dec);
    test_dec.bits[LOW] = -124124;
    from_decimal_to_int(test_dec, &test_int);
    ck_assert_int_eq(test_int, control_int);
}
END_TEST

START_TEST(from_decimal_to_int_4) {
    int test_int = 0;
    int control_int = -INT_MAX;
    decimal test_dec;
    init_dec(&test_dec);
    test_dec.bits[LOW] = -INT_MAX;
    from_decimal_to_int(test_dec, &test_int);
    ck_assert_int_eq(test_int, control_int);
}
END_TEST

START_TEST(from_decimal_to_int_5) {
    int test_int = 0;
    int control_int = 0;
    decimal test_dec;
    init_dec(&test_dec);
    test_dec.bits[LOW] = 0;
    from_decimal_to_int(test_dec, &test_int);
    ck_assert_int_eq(test_int, control_int);
}
END_TEST

START_TEST(from_decimal_to_int_6) {
    int test_int = 0;
    int control_int = 0;
    decimal dec = {0x00000009, 0x00000000, 0x00000000, 0x00010000, 0x0};  // 0.9
    from_decimal_to_int(dec, &test_int);
    ck_assert_int_eq(test_int, control_int);
}
END_TEST

START_TEST(from_decimal_to_int_7) {
    int test_int = 0;
    int control_int = 1;
    decimal dec = {0x0000000F, 0x00000000, 0x00000000, 0x00010000, 0x0};  // 1.5
    from_decimal_to_int(dec, &test_int);
    ck_assert_int_eq(test_int, control_int);
}
END_TEST

START_TEST(from_decimal_to_int_8) {
    int test_int = 0;
    int control_int = -1;
    decimal dec = {0x0000000F, 0x00000000, 0x00000000, 0x80010000, 0x0};  // -1.5
    from_decimal_to_int(dec, &test_int);
    ck_assert_int_eq(test_int, control_int);
}
END_TEST

START_TEST(from_decimal_to_int_9) {
    int test_int = CONVERTING_ERROR;
    int control_int = CONVERTING_ERROR;
    decimal test_dec;
    init_dec(&test_dec);
    test_dec.bits[LOW] = INT_MAX;
    test_dec.bits[MID] = 15;
    from_decimal_to_int(test_dec, &test_int);
    ck_assert_int_eq(test_int, control_int);
}
END_TEST

START_TEST(dop_code_convert_test) {
    decimal test_dec, control_dec;
    init_dec(&test_dec);
    init_dec(&control_dec);
    test_dec.bits[LOW] = 0x1;
    test_dec = dop_code(test_dec);
    control_dec.bits[LOW] = 0xFFFFFFFF;
    control_dec.bits[MID] = 0xFFFFFFFF;
    control_dec.bits[HIGH] = 0xFFFFFFFF;
    control_dec.value_type = D_DOP_CODE;
    ck_assert_decimal_eq(test_dec, control_dec);

    test_dec.bits[LOW] = 0xFFFFFFFF;
    test_dec.bits[MID] = 0xFFFFFFFF;
    test_dec.bits[HIGH] = 0xFFFFFFFF;
    test_dec = dop_code(test_dec);
    control_dec.bits[LOW] = 0x1;
    control_dec.bits[MID] = 0x0;
    control_dec.bits[HIGH] = 0x0;
    ck_assert_decimal_eq(test_dec, control_dec);
}
END_TEST

START_TEST(ten_div_test) {
    decimal test_dec, control_dec;
    init_dec(&test_dec);
    init_dec(&control_dec);
    test_dec.bits[LOW] = 0xFFFFFFFF;
    test_dec.bits[MID] = 0xFFFFFFFF;
    test_dec.bits[HIGH] = 0xFFFFFFFF;
    control_dec.bits[LOW] = 0x99999999;
    control_dec.bits[MID] = 0x99999999;
    control_dec.bits[HIGH] = 0x19999999;
    test_dec = ten_div(test_dec);
    ck_assert_decimal_eq(test_dec, control_dec);
}
END_TEST

START_TEST(truncate_test_1) {
    decimal dec = {0x0000000F, 0x00000000, 0x00000000, 0x00010000, 0x0};          // 1.5
    decimal control_dec = {0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 1
    ck_assert_decimal_eq(decimal_truncate(dec), control_dec);
}
END_TEST

START_TEST(truncate_test_2) {
    decimal dec = {0x0000000F, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -1.5
    decimal control_dec = {0x00000001, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -1
    ck_assert_decimal_eq(decimal_truncate(dec), control_dec);
}
END_TEST

START_TEST(truncate_test_3) {
    decimal dec = {0x00000005, 0x00000000, 0x00000000, 0x00010000, 0x0};          // 0.5
    decimal control_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    ck_assert_decimal_eq(decimal_truncate(dec), control_dec);
}
END_TEST

START_TEST(truncate_test_4) {
    decimal dec = {0x00000005, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -0.5
    decimal control_dec = {0x00000000, 0x00000000, 0x00000000, 0x80000000, 0x0};  // 0
    ck_assert_decimal_eq(decimal_truncate(dec), control_dec);
}
END_TEST

START_TEST(less_norm_1) {
    decimal dec1 = {0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 1
    decimal dec2 = {0x1A1DA4C1, 0x24506695, 0x000006D8, 0x80060000, 0x0};  // -32321312321313211.000001
    ck_assert_int_eq(is_less(dec1, dec2), FALSE);
}
END_TEST

START_TEST(less_norm_2) {
    decimal dec1 = {0x00000001, 0x00000000, 0x00000000, 0x00090000, 0x0};  // 0.000000001
    decimal dec2 = {0x1A1DA4C1, 0x24506695, 0x000006D8, 0x00060000, 0x0};  // 32321312321313211.000001
    ck_assert_int_eq(is_less(dec1, dec2), TRUE);
}
END_TEST

START_TEST(less_norm_3) {
    decimal dec1 = {0x000003E7, 0x00000000, 0x00000000, 0x00080000, 0x0};  // 0.00000999
    decimal dec2 = {0x00000002, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -2
    ck_assert_int_eq(is_less(dec1, dec2), FALSE);
}
END_TEST

START_TEST(less_norm_4) {
    decimal dec1 = {0xDE2FD313, 0x047C5A15, 0x00000000, 0x80040000, 0x0};  // -32323232323232.2323
    decimal dec2 = {0x000003E7, 0x00000000, 0x00000000, 0x000E0000, 0x0};  // 0.00000000000999
    ck_assert_int_eq(is_less(dec1, dec2), TRUE);
}
END_TEST

START_TEST(less_NANs) {
    decimal x, y;
    int res = 1;  // NANs
    init_dec(&x);
    init_dec(&y);
    set_scale(&x, 4);
    set_scale(&y, 25);
    x.bits[LOW] = 0xFFF000C1;
    x.bits[MID] = 0xAAA62621;
    x.bits[HIGH] = 0x0000635A;
    x.value_type = D_NAN;

    y.bits[LOW] = 0xFFFFABCC;
    y.bits[MID] = 0xAAAAAA61;
    y.bits[HIGH] = 0xFFF12345;

    ck_assert_int_eq(res, is_less(y, x));
}
END_TEST

START_TEST(less_INFs) {
    decimal x, y;
    int res = 1;  // INFs
    init_dec(&x);
    init_dec(&y);
    set_scale(&x, 4);
    set_scale(&y, 25);
    x.bits[LOW] = 0xFFF000C1;
    x.bits[MID] = 0xAAA62621;
    x.bits[HIGH] = 0x0000635A;
    x.value_type = D_INFINITY;

    y.bits[LOW] = 0xFFFFABCC;
    y.bits[MID] = 0xAAAAAA61;
    y.bits[HIGH] = 0xFFF12345;

    ck_assert_int_eq(res, is_less(y, x));
}
END_TEST


START_TEST(less_1_INF_2_NEG_INF) {
    decimal x, y;
    int res = 1;  // x < y
    init_dec(&x);
    init_dec(&y);
    set_scale(&x, 4);
    set_scale(&y, 25);
    x.bits[LOW] = 0xFFF000C1;
    x.bits[MID] = 0xAAA62621;
    x.bits[HIGH] = 0x0000635A;
    x.value_type = D_NEGATIVE_INFINITY;

    y.bits[LOW] = 0xFFFFABCC;
    y.bits[MID] = 0xAAAAAA61;
    y.bits[HIGH] = 0xFFF12345;
    y.value_type = D_INFINITY;

    ck_assert_int_eq(res, is_less(y, x));
}
END_TEST

START_TEST(less_NEG_INFs) {
    decimal x, y;
    int res = 1;  //
    init_dec(&x);
    init_dec(&y);
    set_scale(&x, 4);
    set_scale(&y, 25);
    x.bits[LOW] = 0xFFF000C1;
    x.bits[MID] = 0xAAA62621;
    x.bits[HIGH] = 0x0000635A;
    x.value_type = D_NEGATIVE_INFINITY;

    y.bits[LOW] = 0xFFFFABCC;
    y.bits[MID] = 0xAAAAAA61;
    y.bits[HIGH] = 0xFFF12345;
    y.value_type = D_NEGATIVE_INFINITY;

    ck_assert_int_eq(res, is_less(y, x));
}
END_TEST

START_TEST(equal_norm_1) {
    decimal dec1 = {0x00000001, 0x00000000, 0x00000000, 0x00030000, 0x0};  // 0.001
    decimal dec2 = {0x00000001, 0x00000000, 0x00000000, 0x00030000, 0x0};  // 0.001
    ck_assert_int_eq(is_equal(dec1, dec2), TRUE);
}
END_TEST

START_TEST(equal_norm_2) {
    decimal dec1 = {0x00002EE0, 0x00000000, 0x00000000, 0x00030000, 0x0};  // 12.000
    decimal dec2 = {0x0000000C, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 12
    ck_assert_int_eq(is_equal(dec1, dec2), TRUE);
}
END_TEST

START_TEST(equal_norm_3) {
    decimal x, y;
    int res = 0;  // x = y
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 4800000;
    x.bits[MID] = 0x00000000;
    x.bits[HIGH] = 0x00000000;
    set_scale(&x, 5);
    x = set_sign(x);

    y.bits[LOW] = 480;
    y.bits[MID] = 0x00000000;
    y.bits[HIGH] = 0x00000000;
    set_scale(&y, 1);
    y = set_sign(y);

    ck_assert_int_eq(res, is_equal(x, y));
}
END_TEST

START_TEST(equal_norm_4) {
    decimal x, y;
    int res = 1;  // x != y
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 4800000;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0x00000000;
    set_scale(&x, 5);

    y.bits[LOW] = 480;
    y.bits[MID] = 0x00000000;
    y.bits[HIGH] = 0x00000000;
    set_scale(&y, 1);
    y = set_sign(y);

    ck_assert_int_eq(res, is_equal(x, y));
}
END_TEST

START_TEST(equal_INFs) {
    decimal x, y;
    int res = 1;  // x != y
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 4800000;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0x00000000;
    set_scale(&x, 5);
    x.value_type = D_INFINITY;

    y.bits[LOW] = 480;
    y.bits[MID] = 0x00000000;
    y.bits[HIGH] = 0x00000000;
    set_scale(&y, 1);
    y = set_sign(y);
    y.value_type = D_INFINITY;

    ck_assert_int_eq(res, is_equal(x, y));
}
END_TEST

START_TEST(equal_NEG_INFs) {
    decimal x, y;
    int res = 1;  // x != y
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 4800000;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0x00000000;
    set_scale(&x, 5);
    x.value_type = D_NEGATIVE_INFINITY;

    y.bits[LOW] = 480;
    y.bits[MID] = 0x00000000;
    y.bits[HIGH] = 0x00000000;
    set_scale(&y, 1);
    y = set_sign(y);
    y.value_type = D_NEGATIVE_INFINITY;

    ck_assert_int_eq(res, is_equal(x, y));
}
END_TEST

START_TEST(equal_NANs) {
    decimal x, y;
    int res = 1;  // x != y
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 4800000;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0x00000000;
    set_scale(&x, 5);
    x.value_type = D_NAN;

    y.bits[LOW] = 480;
    y.bits[MID] = 0x00000000;
    y.bits[HIGH] = 0x00000000;
    set_scale(&y, 1);
    y = set_sign(y);
    y.value_type = D_NAN;

    ck_assert_int_eq(res, is_equal(x, y));
}
END_TEST

START_TEST(not_equal_norm_1) {
    decimal x, y;
    int res = 1;  // x = y
    init_dec(&x);
    init_dec(&y);
    set_scale(&x, 10);
    set_scale(&y, 10);
    x.bits[LOW] = 0x00000012;
    x.bits[MID] = 0x00000000;
    x.bits[HIGH] = 0xFFFF0000;

    y.bits[LOW] = 0x000000012;
    y.bits[MID] = 0x00000000;
    y.bits[HIGH] = 0xFFFF0000;

    ck_assert_int_eq(res, is_not_equal(x, y));
}
END_TEST

START_TEST(not_equal_norm_2) {
    decimal x, y;
    int res = 1;  // x = y
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 120;
    x.bits[MID] = 0x00000000;
    x.bits[HIGH] = 0x00000000;
    set_scale(&x, 1);

    y.bits[LOW] = 12;
    y.bits[MID] = 0x00000000;
    y.bits[HIGH] = 0x00000000;

    ck_assert_int_eq(res, is_not_equal(x, y));
}
END_TEST

START_TEST(not_equal_norm_3) {
    decimal x, y;
    int res = 1;  // x = y
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 4800000;
    x.bits[MID] = 0x00000000;
    x.bits[HIGH] = 0x00000000;
    set_scale(&x, 5);
    x = set_sign(x);

    y.bits[LOW] = 480;
    y.bits[MID] = 0x00000000;
    y.bits[HIGH] = 0x00000000;
    set_scale(&y, 1);
    y = set_sign(y);

    ck_assert_int_eq(res, is_not_equal(x, y));
}
END_TEST

START_TEST(not_equal_norm_4) {
    decimal x, y;
    int res = 0;  // x != y
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 4800000;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0x00000000;
    set_scale(&x, 5);

    y.bits[LOW] = 480;
    y.bits[MID] = 0x00000000;
    y.bits[HIGH] = 0x00000000;
    set_scale(&y, 1);
    y = set_sign(y);

    ck_assert_int_eq(res, is_not_equal(x, y));
}
END_TEST

START_TEST(not_equal_INFs) {
    decimal x, y;
    int res = 0;  // x != y
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 4800000;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0x00000000;
    set_scale(&x, 5);
    x.value_type = D_INFINITY;

    y.bits[LOW] = 480;
    y.bits[MID] = 0x00000000;
    y.bits[HIGH] = 0x00000000;
    set_scale(&y, 1);
    y = set_sign(y);
    y.value_type = D_INFINITY;

    ck_assert_int_eq(res, is_not_equal(x, y));
}
END_TEST

START_TEST(not_equal_NEG_INFs) {
    decimal x, y;
    int res = 0;  // x != y
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 4800000;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0x00000000;
    set_scale(&x, 5);
    x.value_type = D_NEGATIVE_INFINITY;

    y.bits[LOW] = 480;
    y.bits[MID] = 0x00000000;
    y.bits[HIGH] = 0x00000000;
    set_scale(&y, 1);
    y = set_sign(y);
    y.value_type = D_NEGATIVE_INFINITY;

    ck_assert_int_eq(res, is_not_equal(x, y));
}
END_TEST

START_TEST(not_equal_NANs) {
    decimal x, y;
    int res = 0;  // x != y
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 4800000;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0x00000000;
    set_scale(&x, 5);
    x.value_type = D_NAN;

    y.bits[LOW] = 480;
    y.bits[MID] = 0x00000000;
    y.bits[HIGH] = 0x00000000;
    set_scale(&y, 1);
    y = set_sign(y);
    y.value_type = D_NAN;

    ck_assert_int_eq(res, is_not_equal(x, y));
}
END_TEST

START_TEST(less_equal_norm_1) {
    decimal x, y;
    int res = 1;  // x > y
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 4800000;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0x00000000;
    set_scale(&x, 5);

    y.bits[LOW] = 480;
    y.bits[MID] = 0x00000000;
    y.bits[HIGH] = 0x00000000;
    set_scale(&y, 1);

    ck_assert_int_eq(res, is_less_or_equal(x, y));
}
END_TEST

START_TEST(less_equal_norm_2) {
    decimal x, y;
    int res = 1;  // x > y
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 4800000;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0xFFF44400;
    set_scale(&x, 3);

    y.bits[LOW] = 480;
    y.bits[MID] = 0x00000000;
    y.bits[HIGH] = 0x00000000;
    set_scale(&y, 1);

    ck_assert_int_eq(res, is_less_or_equal(x, y));
}
END_TEST

START_TEST(less_equal_norm_3) {
    decimal x, y;
    int res = 0;  // x = y
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 480;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0xFFF00000;
    set_scale(&x, 1);

    y.bits[LOW] = 480;
    y.bits[MID] = 0x01500000;
    y.bits[HIGH] = 0xFFF00000;
    set_scale(&y, 1);

    ck_assert_int_eq(res, is_less_or_equal(x, y));
}
END_TEST

START_TEST(less_equal_norm_4) {
    decimal x, y;
    int res = 0;  // x < y
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 480;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0xFFF00000;
    set_scale(&x, 1);
    x = set_sign(x);
    y.bits[LOW] = 480;
    y.bits[MID] = 0x01500000;
    y.bits[HIGH] = 0xF3300000;
    set_scale(&y, 1);
    ck_assert_int_eq(res, is_less_or_equal(x, y));
}
END_TEST

START_TEST(less_equal_norm_5) {
    decimal dec1 = {0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 3
    decimal dec2 = {0x00000002, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 2
    ck_assert_int_eq(is_less_or_equal(dec1, dec2), FALSE);
}
END_TEST

START_TEST(less_equal_INFs) {
    decimal x, y;
    int res = 1;  // x and y are INFs
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 480;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0xFFF00000;
    x.value_type = D_INFINITY;

    y.bits[LOW] = 480;
    y.bits[MID] = 0x01500000;
    y.bits[HIGH] = 0xF3300000;
    set_scale(&y, 1);
    y.value_type = D_INFINITY;

    ck_assert_int_eq(res, is_less_or_equal(x, y));
}
END_TEST

START_TEST(less_equal_NEG_INFs) {
    decimal x, y;
    int res = 1;  // x and y are negative INFs
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 480;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0xFFF00000;
    x.value_type = D_NEGATIVE_INFINITY;

    y.bits[LOW] = 480;
    y.bits[MID] = 0x01500000;
    y.bits[HIGH] = 0xF3300000;
    set_scale(&y, 1);
    y.value_type = D_NEGATIVE_INFINITY;

    ck_assert_int_eq(res, is_less_or_equal(x, y));
}
END_TEST

START_TEST(less_equal_NANs) {
    decimal x, y;
    int res = 1;  // x and y are NANs
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 480;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0xFFF00000;
    x.value_type = D_NAN;

    y.bits[LOW] = 480;
    y.bits[MID] = 0x01500000;
    y.bits[HIGH] = 0xF3300000;
    set_scale(&y, 1);
    y.value_type = D_NAN;

    ck_assert_int_eq(res, is_less_or_equal(x, y));
}
END_TEST

START_TEST(greater_norm_1) {
    decimal dec1 = {0x50BCAC9A, 0x00000000, 0x00000000, 0x00030000, 0x0};  // 1354542.234
    decimal dec2 = {0x74ED9B81, 0xE758E637, 0x00000019, 0x00060000, 0x0};  // 477838928989310.000001
    ck_assert_int_eq(is_greater(dec1, dec2), FALSE);
}
END_TEST

START_TEST(greater_norm_2) {
    decimal dec1 = {0x000007F2, 0x00000000, 0x00000000, 0x00080000, 0x0};  // 0.00002034
    decimal dec2 = {0x74ED9B81, 0xE758E637, 0x00000019, 0x80060000, 0x0};  // -477838928989310.000001
    ck_assert_int_eq(is_greater(dec1, dec2), TRUE);
}
END_TEST

START_TEST(greater_norm_3) {
    decimal dec1 = {0x000007F2, 0x00000000, 0x00000000, 0x80080000, 0x0};  // -0.00002034
    decimal dec2 = {0x23FF5B81, 0x00A9C7D5, 0x00000000, 0x80060000, 0x0};  // -47788989310.000001
    ck_assert_int_eq(is_greater(dec1, dec2), TRUE);
}
END_TEST

START_TEST(greater_norm_4) {
    decimal dec1 = {0x00000001, 0x00000000, 0x00000000, 0x800E0000, 0x0};  // -0.00000000000001
    decimal dec2 = {0x1A1DA4C1, 0x24506695, 0x000006D8, 0x00060000, 0x0};  // 32321312321313211.000001
    ck_assert_int_eq(is_greater(dec1, dec2), FALSE);
}
END_TEST

START_TEST(greater_norm_5) {
    decimal dec1 = {0x00000001, 0x00000000, 0x00000000, 0x000E0000, 0x0};  // 0.00000000000001
    decimal dec2 = {0x1A1DA4C1, 0x24506695, 0x000006D8, 0x80060000, 0x0};  // -32321312321313211.000001
    ck_assert_int_eq(is_greater(dec1, dec2), TRUE);
}
END_TEST

START_TEST(greater_norm_6) {
    decimal dec1 = {0x00000002, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 2
    decimal dec2 = {0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 3
    ck_assert_int_eq(is_greater(dec1, dec2), FALSE);
}
END_TEST

START_TEST(greater_INFs) {
    decimal x, y;
    int res = 1;
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 480;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0xFFFFFFFF;
    set_scale(&x, 15);
    x.value_type = D_INFINITY;

    y.bits[LOW] = 480;
    y.bits[MID] = 0x01500000;
    y.bits[HIGH] = 0xF3300000;
    y.value_type = D_INFINITY;

    ck_assert_int_eq(res, is_greater(x, y));
}
END_TEST

START_TEST(greater_NEG_INFs) {
    decimal x, y;
    int res = 1;
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 480;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0xFFFFFFFF;
    set_scale(&x, 15);
    x.value_type = D_NEGATIVE_INFINITY;

    y.bits[LOW] = 480;
    y.bits[MID] = 0x01500000;
    y.bits[HIGH] = 0xF3300000;
    y.value_type = D_NEGATIVE_INFINITY;

    ck_assert_int_eq(res, is_greater(x, y));
}
END_TEST

START_TEST(greater_NANs) {
    decimal x, y;
    int res = 1;
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 480;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0xFFFFFFFF;
    set_scale(&x, 15);
    x.value_type = D_NAN;

    y.bits[LOW] = 480;
    y.bits[MID] = 0x01500000;
    y.bits[HIGH] = 0xF3300000;
    y.value_type = D_NAN;

    ck_assert_int_eq(res, is_greater(x, y));
}
END_TEST

START_TEST(greater_1_INF_2_NEG_INF) {
    decimal x, y;
    int res = 0;
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 480;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0xFFFFFFFF;
    set_scale(&x, 15);
    x.value_type = D_INFINITY;

    y.bits[LOW] = 480;
    y.bits[MID] = 0x01500000;
    y.bits[HIGH] = 0xF3300000;
    y.value_type = D_NEGATIVE_INFINITY;

    ck_assert_int_eq(res, is_greater(x, y));
}
END_TEST

START_TEST(normalization_test) {
    decimal test_dec = {0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0};
    decimal tmp_dec = {0x0000000F, 0x00000000, 0x00000000, 0x00010000, 0x0};
    decimal control_dec = {0x0000000A, 0x00000000, 0x00000000, 0x00010000, 0x0};
    normalize(&test_dec, &tmp_dec);
    ck_assert_decimal_eq(test_dec, control_dec);
}
END_TEST

START_TEST(normalization_test_1) {
    decimal test_dec = {0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0};
    decimal tmp_dec = {0x00000001, 0x00000000, 0x00000000, 0x00090000, 0x0};
    decimal control_dec = {0x3B9ACA00, 0x00000000, 0x00000000, 0x00090000, 0x0};
    normalize(&test_dec, &tmp_dec);
    ck_assert_decimal_eq(test_dec, control_dec);
}
END_TEST

START_TEST(normalization_test_2) {
    decimal test_dec = {0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x0};
    // 79228162514264337593543950334
    decimal tmp_dec = {0x00000001, 0x00000000, 0x00000000, 0x00090000, 0x0};
    decimal control_dec = {0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x0};
    // 79228162514264337593543950334
    normalize(&test_dec, &tmp_dec);
    ck_assert_decimal_eq(test_dec, control_dec);
}
END_TEST

START_TEST(add_test_1) {
    decimal dec1 = {0x00000005, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 5
    decimal dec2 = {0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 3
    decimal control_dec = {0x00000008, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 8
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_2) {
    decimal dec1 = {0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 3
    decimal dec2 = {0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 3
    decimal control_dec = {0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 6
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_3) {
    decimal dec1 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x0};
    // 79228162514264337593543950335
    decimal dec2 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000, 0x0};
    // -79228162514264337593543950335
    decimal control_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_4) {
    decimal dec1 = {0x00000001, 0x00000000, 0x00000000, 0x001C0000, 0x0};
    // 0.0000000000000000000000000001
    decimal dec2 = {0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 3
    decimal control_dec = {0x30000001, 0xBA6F0723, 0x60EF6B1A, 0x001C0000, 0x0};
    // 3.0000000000000000000000000001
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_5) {
    decimal dec1 = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    decimal dec2 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x0};
    // 79228162514264337593543950335
    decimal control_dec = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x0};
    // 79228162514264337593543950335
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_6) {
    decimal dec1 = {0x00000007, 0x00000000, 0x00000000, 0x80000000, 0x0};         // -7
    decimal dec2 = {0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 3
    decimal control_dec = {0x00000004, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -4
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_7) {
    decimal dec1 = {0x00000003, 0x00000000, 0x00000000, 0x80000000, 0x0};         // -3
    decimal dec2 = {0x00000007, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 7
    decimal control_dec = {0x00000004, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 4
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_8) {
    decimal dec1 = {0x00000007, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 7
    decimal dec2 = {0x00000003, 0x00000000, 0x00000000, 0x80000000, 0x0};         // -3
    decimal control_dec = {0x00000004, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 4
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_9) {
    decimal dec1 = {0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 3
    decimal dec2 = {0x00000007, 0x00000000, 0x00000000, 0x80000000, 0x0};         // -7
    decimal control_dec = {0x00000004, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -4
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_10) {
    decimal dec1 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x0};
    // 79228162514264337593543950335
    decimal dec2 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000, 0x0};
    // -79228162514264337593543950335
    decimal control_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_11) {
    decimal x = {0x000003E8, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 1000
    decimal y = {0x00000001, 0x00000000, 0x00000000, 0x001C0000, 0x0};
    // 0.0000000000000000000000000001
    decimal control_dec = {0x10000000, 0x3E250261, 0x204FCE5E, 0x00190000, 0x0};
    // 1000.0000000000000000000000000
    ck_assert_decimal_eq(decimal_add(x, y), control_dec);
}
END_TEST

START_TEST(add_test_12) {
    decimal dec1 = {0x00000005, 0x00000000, 0x00000000, 0x001C0000, 0x0};
    // 0.0000000000000000000000000005
    decimal dec2 = {0x0000000A, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 10
    decimal control_dec = {0x10000000, 0x3E250261, 0x204FCE5E, 0x001B0000, 0x0};
    // 10.000000000000000000000000000
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_13) {
    decimal dec1 = {0x0000000F, 0x00000000, 0x00000000, 0x001C0000, 0x0};
    // 0.0000000000000000000000000015
    decimal dec2 = {0x0000000A, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 10
    decimal control_dec = {0x10000002, 0x3E250261, 0x204FCE5E, 0x001B0000, 0x0};
    // 10.000000000000000000000000002
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_14) {
    decimal dec1 = {0x00000019, 0x00000000, 0x00000000, 0x001C0000, 0x0};
    // 0.0000000000000000000000000025
    decimal dec2 = {0x0000000A, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 10
    decimal control_dec = {0x10000002, 0x3E250261, 0x204FCE5E, 0x001B0000, 0x0};
    // 10.000000000000000000000000002
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_15) {
    decimal dec1 = {0x00000023, 0x00000000, 0x00000000, 0x001C0000, 0x0};
    // 0.0000000000000000000000000035
    decimal dec2 = {0x0000000A, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 10
    decimal control_dec = {0x10000004, 0x3E250261, 0x204FCE5E, 0x001B0000, 0x0};
    // 10.000000000000000000000000004
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_16) {
    decimal dec1 = {0x0000002D, 0x00000000, 0x00000000, 0x001C0000, 0x0};
    // 0.0000000000000000000000000045
    decimal dec2 = {0x0000000A, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 10
    decimal control_dec = {0x10000004, 0x3E250261, 0x204FCE5E, 0x001B0000, 0x0};
    // 10.000000000000000000000000004
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_17) {
    decimal dec1 = {0x00000037, 0x00000000, 0x00000000, 0x001C0000, 0x0};
    // 0.0000000000000000000000000055
    decimal dec2 = {0x0000000A, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 10
    decimal control_dec = {0x10000006, 0x3E250261, 0x204FCE5E, 0x001B0000, 0x0};
    // 10.000000000000000000000000006
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_18) {
    decimal dec1 = {0x00000041, 0x00000000, 0x00000000, 0x001C0000, 0x0};
    // 0.0000000000000000000000000065
    decimal dec2 = {0x0000000A, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 10
    decimal control_dec = {0x10000006, 0x3E250261, 0x204FCE5E, 0x001B0000, 0x0};
    // 10.000000000000000000000000006
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_19) {
    decimal dec1 = {0x0000004B, 0x00000000, 0x00000000, 0x001C0000, 0x0};
    // 0.0000000000000000000000000075
    decimal dec2 = {0x0000000A, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 10
    decimal control_dec = {0x10000008, 0x3E250261, 0x204FCE5E, 0x001B0000, 0x0};
    // 10.000000000000000000000000008
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_20) {
    decimal dec1 = {0x00000055, 0x00000000, 0x00000000, 0x001C0000, 0x0};
    // 0.0000000000000000000000000085
    decimal dec2 = {0x0000000A, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 10
    decimal control_dec = {0x10000008, 0x3E250261, 0x204FCE5E, 0x001B0000, 0x0};
    // 10.000000000000000000000000008
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_21) {
    decimal dec1 = {0x0000005F, 0x00000000, 0x00000000, 0x001C0000, 0x0};
    // 0.0000000000000000000000000095
    decimal dec2 = {0x0000000A, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 10
    decimal control_dec = {0x1000000A, 0x3E250261, 0x204FCE5E, 0x001B0000, 0x0};
    // 10.000000000000000000000000010
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_22) {
    decimal dec1 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x0};
    // 79228162514264337593543950335
    decimal dec2 = {0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 1
    decimal control_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, D_INFINITY};  // 0
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_23) {
    decimal dec1 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000, 0x0};
    // -79228162514264337593543950335
    decimal dec2 = {0x00000001, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -1
    decimal control_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, D_NEGATIVE_INFINITY};  // 0
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_24) {
    decimal dec1 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000, D_NAN};
    // -79228162514264337593543950335
    decimal dec2 = {0x00000001, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -1
    decimal control_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, D_NAN};  // 0
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_25) {
    decimal dec1 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000, 0x0};
    // -79228162514264337593543950335
    decimal dec2 = {0x00000001, 0x00000000, 0x00000000, 0x80000000, D_NAN};  // -1
    decimal control_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, D_NAN};  // 0
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_26) {
    decimal dec1 = {0x00000000, 0x00000000, 0x00000000, 0x00000000, D_INFINITY};
    // -79228162514264337593543950335
    decimal dec2 = {0x00000001, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -1
    decimal control_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, D_INFINITY};  // 0
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(add_test_27) {
    decimal dec1 = {0x00000000, 0x00000000, 0x00000000, 0x00000000, D_NEGATIVE_INFINITY};
    // -79228162514264337593543950335
    decimal dec2 = {0x00000001, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -1
    decimal control_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, D_NEGATIVE_INFINITY};  // 0
    ck_assert_decimal_eq(decimal_add(dec1, dec2), control_dec);
}
END_TEST

START_TEST(sub_test_1) {
    decimal dec1 = {0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 1
    decimal dec2 = {0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 1
    decimal control_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    ck_assert_decimal_eq(decimal_sub(dec1, dec2), control_dec);
}
END_TEST

START_TEST(sub_test_2) {
    decimal dec1 = {0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 3
    decimal dec2 = {0x00000002, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 2
    decimal control_dec = {0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 1
    ck_assert_decimal_eq(decimal_sub(dec1, dec2), control_dec);
}
END_TEST

START_TEST(sub_test_3) {
    decimal dec1 = {0x00000003, 0x00000000, 0x00000000, 0x80000000, 0x0};         // -3
    decimal dec2 = {0x00000002, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 2
    decimal control_dec = {0x00000005, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -5
    ck_assert_decimal_eq(decimal_sub(dec1, dec2), control_dec);
}
END_TEST

START_TEST(sub_test_4) {
    decimal dec1 = {0x00000002, 0x00000000, 0x00000000, 0x80000000, 0x0};         // -2
    decimal dec2 = {0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 3
    decimal control_dec = {0x00000005, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -5
    ck_assert_decimal_eq(decimal_sub(dec1, dec2), control_dec);
}
END_TEST

START_TEST(sub_test_5) {
    decimal dec1 = {0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 3
    decimal dec2 = {0x00000002, 0x00000000, 0x00000000, 0x80000000, 0x0};         // -2
    decimal control_dec = {0x00000005, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 5
    ck_assert_decimal_eq(decimal_sub(dec1, dec2), control_dec);
}
END_TEST

START_TEST(sub_test_6) {
    decimal dec1 = {0x00000002, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 2
    decimal dec2 = {0x00000003, 0x00000000, 0x00000000, 0x80000000, 0x0};         // -3
    decimal control_dec = {0x00000005, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 5
    ck_assert_decimal_eq(decimal_sub(dec1, dec2), control_dec);
}
END_TEST

START_TEST(sub_test_7) {
    decimal dec1 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x0};
    // 79228162514264337593543950335
    decimal dec2 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x0};
    // 79228162514264337593543950335
    decimal control_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    ck_assert_decimal_eq(decimal_sub(dec1, dec2), control_dec);
}
END_TEST

START_TEST(sub_test_8) {
    decimal dec1 = {0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 1
    decimal dec2 = {0x00000001, 0x00000000, 0x00000000, 0x00190000, 0x0};
    // 0.0000000000000000000000001
    decimal control_dec = {0x49FFFFFF, 0x16140148, 0x00084595, 0x00190000, 0x0};
    // 0.9999999999999999999999999
    ck_assert_decimal_eq(decimal_sub(dec1, dec2), control_dec);
}
END_TEST

START_TEST(overflow_control_test) {
    decimal x = {0x0, 0x0, 0xFFFFFFFF, 0x0, 0x0};
    ck_assert_int_eq(overflow_control(x), 0);
    x.bits[HIGH] = 0x2FFFFFFF;
    ck_assert_int_eq(overflow_control(x), 0);
    x.bits[HIGH] = 0x1FFFFFFF;
    ck_assert_int_eq(overflow_control(x), 0);
    x.bits[HIGH] = 0x0FFFFFFF;
    ck_assert_int_eq(overflow_control(x), 1);
}
END_TEST

START_TEST(greater_equal_norm_1) {
    decimal x, y;
    int res = 0;  // x > y
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 480;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0xFFFFFFFF;

    y.bits[LOW] = 480;
    y.bits[MID] = 0x01500000;
    y.bits[HIGH] = 0xF3300000;

    ck_assert_int_eq(res, is_greater_or_equal(x, y));
}
END_TEST

START_TEST(greater_equal_norm_2) {
    decimal x, y;
    int res = 0;  // x = y
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 480;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0xFFFFFFFF;

    y.bits[LOW] = 480;
    y.bits[MID] = 0x01500000;
    y.bits[HIGH] = 0xFFFFFFFF;

    ck_assert_int_eq(res, is_greater_or_equal(x, y));
}
END_TEST

START_TEST(greater_equal_norm_3) {
    decimal x, y;
    int res = 1;  // x < y
    init_dec(&x);
    init_dec(&y);
    x.bits[LOW] = 480;
    x.bits[MID] = 0x01500000;
    x.bits[HIGH] = 0xFFFFFFFF;
    set_scale(&x, 9);

    y.bits[LOW] = 480;
    y.bits[MID] = 0x01500000;
    y.bits[HIGH] = 0xFFFFFFFF;

    ck_assert_int_eq(res, is_greater_or_equal(x, y));
}
END_TEST

START_TEST(round_test_1) {
    decimal dec = {0x00000005, 0x00000000, 0x00000000, 0x00010000, 0x0};          // 0.5
    decimal control_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_2) {
    decimal dec = {0x0000000F, 0x00000000, 0x00000000, 0x00010000, 0x0};          // 1.5
    decimal control_dec = {0x00000002, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 2
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_3) {
    decimal dec = {0x00000019, 0x00000000, 0x00000000, 0x00010000, 0x0};          // 2.5
    decimal control_dec = {0x00000002, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 2
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_4) {
    decimal dec = {0x00000023, 0x00000000, 0x00000000, 0x00010000, 0x0};          // 3.5
    decimal control_dec = {0x00000004, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 4
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_5) {
    decimal dec = {0x0000002D, 0x00000000, 0x00000000, 0x00010000, 0x0};          // 4.5
    decimal control_dec = {0x00000004, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 4
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_6) {
    decimal dec = {0x00000037, 0x00000000, 0x00000000, 0x00010000, 0x0};          // 5.5
    decimal control_dec = {0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 6
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_7) {
    decimal dec = {0x00000041, 0x00000000, 0x00000000, 0x00010000, 0x0};          // 6.5
    decimal control_dec = {0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 6
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_8) {
    decimal dec = {0x0000004B, 0x00000000, 0x00000000, 0x00010000, 0x0};          // 7.5
    decimal control_dec = {0x00000008, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 8
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_9) {
    decimal dec = {0x00000055, 0x00000000, 0x00000000, 0x00010000, 0x0};          // 8.5
    decimal control_dec = {0x00000008, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 8
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_10) {
    decimal dec = {0x0000005F, 0x00000000, 0x00000000, 0x00010000, 0x0};          // 9.5
    decimal control_dec = {0x0000000A, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 10
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_11) {
    decimal dec = {0x00000005, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -0.5
    decimal control_dec = {0x00000000, 0x00000000, 0x00000000, 0x80000000, 0x0};  // 0
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_12) {
    decimal dec = {0x0000000F, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -1.5
    decimal control_dec = {0x00000002, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -2
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_13) {
    decimal dec = {0x00000019, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -2.5
    decimal control_dec = {0x00000002, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -2
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_14) {
    decimal dec = {0x00000023, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -3.5
    decimal control_dec = {0x00000004, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -4
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_15) {
    decimal dec = {0x0000002D, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -4.5
    decimal control_dec = {0x00000004, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -4
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_16) {
    decimal dec = {0x00000037, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -5.5
    decimal control_dec = {0x00000006, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -6
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_17) {
    decimal dec = {0x00000041, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -6.5
    decimal control_dec = {0x00000006, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -6
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_18) {
    decimal dec = {0x0000004B, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -7.5
    decimal control_dec = {0x00000008, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -8
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_19) {
    decimal dec = {0x00000055, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -8.5
    decimal control_dec = {0x00000008, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -8
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_20) {
    decimal dec = {0x0000005F, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -9.5
    decimal control_dec = {0x0000000A, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -10
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_21) {
    decimal dec = {0x0000000D, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -1.3
    decimal control_dec = {0x00000001, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -1
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_22) {
    decimal dec = {0x0000000D, 0x00000000, 0x00000000, 0x00010000, 0x0};          // 1.3
    decimal control_dec = {0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 1
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_23) {
    decimal dec = {0x00000011, 0x00000000, 0x00000000, 0x00010000, 0x0};          // 1.7
    decimal control_dec = {0x00000002, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 2
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(round_test_24) {
    decimal dec = {0x00000011, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -1.7
    decimal control_dec = {0x00000002, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -2
    ck_assert_decimal_eq(decimal_round(dec), control_dec);
}
END_TEST

START_TEST(greater_equal_norm_4) {
    decimal dec1 = {0x00002EE0, 0x00000000, 0x00000000, 0x00030000, 0x0};  // 12.000
    decimal dec2 = {0x0000000C, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 12
    ck_assert_int_eq(is_greater_or_equal(dec1, dec2), TRUE);
}
END_TEST

START_TEST(greater_equal_INFs) {
    decimal dec1 = {0x00002EE0, 0x00000000, 0x00000000, 0x00030000, 0x1};  // 12.000
    decimal dec2 = {0x0000000C, 0x00000000, 0x00000000, 0x00000000, 0x1};  // 12
    ck_assert_int_eq(is_greater_or_equal(dec1, dec2), FALSE);
}
END_TEST

START_TEST(greater_equal_NEG_INFs) {
    decimal dec1 = {0x00002EE0, 0x00000000, 0x00000000, 0x00030000, 0x2};  // 12.000
    decimal dec2 = {0x0000000C, 0x00000000, 0x00000000, 0x00000000, 0x2};  // 12
    ck_assert_int_eq(is_greater_or_equal(dec1, dec2), FALSE);
}
END_TEST

START_TEST(greater_equal_NANs) {
    decimal dec1 = {0x00002EE0, 0x00000000, 0x00000000, 0x00030000, 0x3};  // 12.000
    decimal dec2 = {0x0000000C, 0x00000000, 0x00000000, 0x00000000, 0x3};  // 12
    ck_assert_int_eq(is_greater_or_equal(dec1, dec2), FALSE);
}
END_TEST

START_TEST(greater_equal_1_INF_2_NEG_INF) {
    decimal dec1 = {0x00002EE0, 0x00000000, 0x00000000, 0x00030000, 0x1};  // 12.000
    decimal dec2 = {0x0000000C, 0x00000000, 0x00000000, 0x00000000, 0x2};  // 12
    ck_assert_int_eq(is_greater_or_equal(dec1, dec2), TRUE);
}
END_TEST

START_TEST(mul_norm_1) {
    decimal dec1 = {0x000000AA, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 170
    decimal dec2 = {0x0003F7A3, 0x00000000, 0x00000000, 0x00010000, 0x0};         // 26000.3
    decimal control_dec = {0x02A2723E, 0x00000000, 0x00000000, 0x00010000, 0x0};  // 4420051.0
    ck_assert_decimal_eq(decimal_mul(dec1, dec2), control_dec);
}
END_TEST

START_TEST(mul_norm_2) {
    decimal dec1 = {0xC0E79423, 0x0000037F, 0x00000000, 0x00060000, 0x0};  // 3847232.132131
    decimal dec2 = {0x138A6875, 0x00000000, 0x00000000, 0x00010000, 0x0};  // 32783781.3
    decimal control_dec = {0xB1E2EBFF, 0x5F9CAC95, 0x00000044, 0x00070000, 0x0};
    // 126126816830115.4069503
    ck_assert_decimal_eq(decimal_mul(dec1, dec2), control_dec);
}
END_TEST

START_TEST(mul_norm_3) {
    decimal dec1 = {0x00020423, 0x00000000, 0x00000000, 0x80060000, 0x0};         // -0.132131
    decimal dec2 = {0x138A6875, 0x00000000, 0x00000000, 0x80010000, 0x0};         // -32783781.3
    decimal control_dec = {0xA6781BFF, 0x00002765, 0x00000000, 0x00070000, 0x0};  // 4331753.8069503
    ck_assert_decimal_eq(decimal_mul(dec1, dec2), control_dec);
}
END_TEST

START_TEST(mul_norm_4) {
    decimal dec1 = {0x00000001, 0x00000000, 0x00000000, 0x00060000, 0x0};         // 0.000001
    decimal dec2 = {0x6592E003, 0x00000117, 0x00000000, 0x000B0000, 0x0};         // 12.00000000003
    decimal control_dec = {0x6592E003, 0x00000117, 0x00000000, 0x00110000, 0x0};  // 0.00001200000000003
    ck_assert_decimal_eq(decimal_mul(dec1, dec2), control_dec);
}
END_TEST

START_TEST(mul_overflow) {
    decimal dec1 = {0x9F05E2AC, 0xF10F1206, 0x0000007D, 0x00040000, 0x0};  // 232321313132113231.1212
    decimal dec2 = {0x714F71DB, 0x007227FE, 0x00000000, 0x00050000, 0x0};  // 321321211212.23131
    decimal control_dec = {0x8F99899A, 0xC8DBCDE4, 0xF134D6C4, 0x00000000, 0x0};
    // 74649765726026683039935465882
    ck_assert_decimal_eq(decimal_mul(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_1) {
    decimal dec1 = {0x0000000A, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 10
    decimal dec2 = {0x00000002, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 2
    decimal control_dec = {0x00000005, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 5
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_2) {
    decimal dec1 = {0x0000007D, 0x00000000, 0x00000000, 0x00020000, 0x0};  // 1.25
    decimal dec2 = {0x00000087, 0x00000000, 0x00000000, 0x00010000, 0x0};  // 13.5
    decimal control_dec = {0x4897B426, 0x05C10E71, 0x02FDE871, 0x001C0000, 0x0};
    // 0.0925925925925925925925925926
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_3) {
    decimal dec1 = {0x0000007D, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 125
    decimal dec2 = {0x00000087, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 135
    decimal control_dec = {0xD5ED097B, 0x398A906C, 0x1DEB146A, 0x001C0000, 0x0};
    // 0.9259259259259259259259259259
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_4) {
    decimal dec1 = {0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 1
    decimal dec2 = {0x00000001, 0x00000000, 0x00000000, 0x001C0000, 0x0};
    // 0.0000000000000000000000000001
    decimal control_dec = {0x10000000, 0x3E250261, 0x204FCE5E, 0x00000000, 0x0};
    // 10000000000000000000000000000
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_5) {
    decimal dec1 = {0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 3
    decimal dec2 = {0x00000002, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 2
    decimal control_dec = {0x0000000F, 0x00000000, 0x00000000, 0x00010000, 0x0};  // 1.5
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_6) {
    decimal dec1 = {0x00000003, 0x00000000, 0x00000000, 0x80000000, 0x0};         // -3
    decimal dec2 = {0x00000002, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 2
    decimal control_dec = {0x0000000F, 0x00000000, 0x00000000, 0x80010000, 0x0};  // -1.5
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_7) {
    decimal dec1 = {0x00000002, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 2
    decimal dec2 = {0x00000003, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -3
    decimal control_dec = {0x0AAAAAAB, 0x296E0196, 0x158A8994, 0x801C0000, 0x0};
    // -0.6666666666666666666666666667
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_8) {
    decimal dec1 = {0x00000003, 0x00000000, 0x00000000, 0x80000000, 0x0};         // -3
    decimal dec2 = {0x00000002, 0x00000000, 0x00000000, 0x80000000, 0x0};         // -2
    decimal control_dec = {0x0000000F, 0x00000000, 0x00000000, 0x00010000, 0x0};  // 1.5
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_9) {
    decimal dec1 = {0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 1
    decimal dec2 = {0x00000009, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 9
    decimal control_dec = {0x571C71C7, 0x06E7AAEE, 0x039716EE, 0x001C0000, 0x0};
    // 0.1111111111111111111111111111
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_10) {
    decimal dec1 = {0x00000002, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 2
    decimal dec2 = {0x00000009, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 9
    decimal control_dec = {0xAE38E38E, 0x0DCF55DC, 0x072E2DDC, 0x001C0000, 0x0};
    // 0.2222222222222222222222222222
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_11) {
    decimal dec1 = {0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 3
    decimal dec2 = {0x00000009, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 9
    decimal control_dec = {0x05555555, 0x14B700CB, 0x0AC544CA, 0x001C0000, 0x0};
    // 0.3333333333333333333333333333
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_12) {
    decimal dec1 = {0x00000004, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 4
    decimal dec2 = {0x00000009, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 9
    decimal control_dec = {0x5C71C71C, 0x1B9EABB9, 0x0E5C5BB8, 0x001C0000, 0x0};
    // 0.4444444444444444444444444444
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_13) {
    decimal dec1 = {0x00000005, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 5
    decimal dec2 = {0x00000009, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 9
    decimal control_dec = {0xB38E38E4, 0x228656A7, 0x11F372A6, 0x001C0000, 0x0};
    // 0.5555555555555555555555555556
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_14) {
    decimal dec1 = {0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 6
    decimal dec2 = {0x00000009, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 9
    decimal control_dec = {0x0AAAAAAB, 0x296E0196, 0x158A8994, 0x001C0000, 0x0};
    // 0.6666666666666666666666666667
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_15) {
    decimal dec1 = {0x00000007, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 7
    decimal dec2 = {0x00000009, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 9
    decimal control_dec = {0x61C71C72, 0x3055AC84, 0x1921A082, 0x001C0000, 0x0};
    // 0.7777777777777777777777777778
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_16) {
    decimal dec1 = {0x00000008, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 8
    decimal dec2 = {0x00000009, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 9
    decimal control_dec = {0xB8E38E39, 0x373D5772, 0x1CB8B770, 0x001C0000, 0x0};
    // 0.8888888888888888888888888889
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_17) {
    decimal dec1 = {0xB9000000, 0xB7320334, 0x0014ADF4, 0x00000000, 0x0};  // 25000000000000000000000000
    decimal dec2 = {0x00000001, 0x00000000, 0x00000000, 0x00030000, 0x0};  // 0.001
    decimal control_dec = {0xA8000000, 0x9B5C85F2, 0x50C783EB, 0x00000000, 0x0};
    // 25000000000000000000000000000
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_18) {
    decimal dec1 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x001C0000, 0x0};
    // 7.9228162514264337593543950335
    decimal dec2 = {0x00000001, 0x00000000, 0x00000000, 0x001C0000, 0x0};
    // 0.0000000000000000000000000001
    decimal control_dec = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x0};
    // 79228162514264337593543950335
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_19) {
    decimal dec1 = {0x0001E363, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 123747
    decimal dec2 = {0x00000007, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 7
    decimal control_dec = {0x2E249249, 0x8FA8CC61, 0x391F045C, 0x00180000, 0x0};
    // 17678.142857142857142857142857
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_20) {
    decimal dec1 = {0x0000FA7B, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 64123
    decimal dec2 = {0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 3
    decimal control_dec = {0x1E555555, 0xC57ED722, 0x45106F23, 0x00180000, 0x0};
    // 21374.333333333333333333333333
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_21) {
    decimal dec1 = {0x00000011, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 17
    decimal dec2 = {0x00000010, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 16
    decimal control_dec = {0x00002981, 0x00000000, 0x00000000, 0x00040000, 0x0};  // 1.0625
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_22) {
    decimal dec1 = {0x00000011, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 17
    decimal dec2 = {0x00000004, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 4
    decimal control_dec = {0x000001A9, 0x00000000, 0x00000000, 0x00020000, 0x0};  // 4.25
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(div_test_23) {
    decimal dec1 = {0x0000000F, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 15
    decimal dec2 = {0x00000004, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 4
    decimal control_dec = {0x00000177, 0x00000000, 0x00000000, 0x00020000, 0x0};  // 3.75
    ck_assert_decimal_eq(decimal_div(dec1, dec2), control_dec);
}
END_TEST

START_TEST(floor_test_1) {
    decimal dec = {0x00000013, 0x00000000, 0x00000000, 0x00010000, 0x0};          // 1.9
    decimal control_dec = {0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 1
    ck_assert_decimal_eq(decimal_floor(dec), control_dec);
}
END_TEST

START_TEST(floor_test_2) {
    decimal dec = {0x0000004F, 0x00000000, 0x00000000, 0x001C0000, 0x0};
    // 0.0000000000000000000000000079
    decimal control_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    ck_assert_decimal_eq(decimal_floor(dec), control_dec);
}
END_TEST

START_TEST(floor_test_3) {
    decimal dec = {0x0000000B, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -1.1
    decimal control_dec = {0x00000002, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -2
    ck_assert_decimal_eq(decimal_floor(dec), control_dec);
}
END_TEST

START_TEST(floor_test_4) {
    decimal dec = {0x0000004F, 0x00000000, 0x00000000, 0x801C0000, 0x0};
    // -0.0000000000000000000000000079
    decimal control_dec = {0x00000001, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -1
    ck_assert_decimal_eq(decimal_floor(dec), control_dec);
}
END_TEST

START_TEST(floor_test_5) {
    decimal dec = {0x00000013, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -1.9
    decimal control_dec = {0x00000002, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -2
    ck_assert_decimal_eq(decimal_floor(dec), control_dec);
}
END_TEST

START_TEST(floor_test_6) {
    decimal dec = {0x00000036, 0x00000000, 0x00000000, 0x00010000, 0x0};          // 5.4
    decimal control_dec = {0x00000005, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 5
    ck_assert_decimal_eq(decimal_floor(dec), control_dec);
}
END_TEST

START_TEST(floor_test_7) {
    decimal dec = {0x00000042, 0x00000000, 0x00000000, 0x00010000, 0x0};          // 6.6
    decimal control_dec = {0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 6
    ck_assert_decimal_eq(decimal_floor(dec), control_dec);
}
END_TEST

START_TEST(floor_test_8) {
    decimal dec = {0x0000004B, 0x00000000, 0x00000000, 0x00010000, 0x0};          // 7.5
    decimal control_dec = {0x00000007, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 8
    ck_assert_decimal_eq(decimal_floor(dec), control_dec);
}
END_TEST

START_TEST(floor_test_9) {
    decimal dec = {0x00000055, 0x00000000, 0x00000000, 0x00010000, 0x0};          // 8.5
    decimal control_dec = {0x00000008, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 8
    ck_assert_decimal_eq(decimal_floor(dec), control_dec);
}
END_TEST

START_TEST(floor_test_10) {
    decimal dec = {0x0000005F, 0x00000000, 0x00000000, 0x00010000, 0x0};          // 9.5
    decimal control_dec = {0x00000009, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 9
    ck_assert_decimal_eq(decimal_floor(dec), control_dec);
}
END_TEST

START_TEST(floor_test_11) {
    decimal dec = {0x00000005, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -0.5
    decimal control_dec = {0x00000001, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -1
    ck_assert_decimal_eq(decimal_floor(dec), control_dec);
}
END_TEST

START_TEST(floor_test_12) {
    decimal dec = {0x0000000F, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -1.5
    decimal control_dec = {0x00000002, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -2
    ck_assert_decimal_eq(decimal_floor(dec), control_dec);
}
END_TEST

START_TEST(floor_test_13) {
    decimal dec = {0x00000019, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -2.5
    decimal control_dec = {0x00000003, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -3
    ck_assert_decimal_eq(decimal_floor(dec), control_dec);
}
END_TEST

START_TEST(floor_test_14) {
    decimal dec = {0x00000023, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -3.5
    decimal control_dec = {0x00000004, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -4
    ck_assert_decimal_eq(decimal_floor(dec), control_dec);
}
END_TEST

START_TEST(floor_test_15) {
    decimal dec = {0x0000002D, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -4.5
    decimal control_dec = {0x00000005, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -5
    ck_assert_decimal_eq(decimal_floor(dec), control_dec);
}
END_TEST

START_TEST(floor_test_16) {
    decimal dec = {0x00000037, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -5.5
    decimal control_dec = {0x00000006, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -6
    ck_assert_decimal_eq(decimal_floor(dec), control_dec);
}
END_TEST

START_TEST(floor_test_17) {
    decimal dec = {0x00000041, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -6.5
    decimal control_dec = {0x00000007, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -7
    ck_assert_decimal_eq(decimal_floor(dec), control_dec);
}
END_TEST

START_TEST(floor_test_18) {
    decimal dec = {0x0000004B, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -7.5
    decimal control_dec = {0x00000008, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -8
    ck_assert_decimal_eq(decimal_floor(dec), control_dec);
}
END_TEST

START_TEST(floor_test_19) {
    decimal dec = {0x00000055, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -8.5
    decimal control_dec = {0x00000009, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -9
    ck_assert_decimal_eq(decimal_floor(dec), control_dec);
}
END_TEST

START_TEST(floor_test_20) {
    decimal dec = {0x0000005F, 0x00000000, 0x00000000, 0x80010000, 0x0};          // -9.5
    decimal control_dec = {0x0000000A, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -10
    ck_assert_decimal_eq(decimal_floor(dec), control_dec);
}
END_TEST

START_TEST(mod_test_1) {
    decimal dec1 = {0x00000005, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 5
    decimal dec2 = {0x00000004, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 4
    decimal control_dec = {0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 1
    ck_assert_decimal_eq(decimal_mod(dec1, dec2), control_dec);
}
END_TEST

START_TEST(mod_test_2) {
    decimal dec1 = {0x00000022, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 34
    decimal dec2 = {0x0000000A, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 10
    decimal control_dec = {0x00000004, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 4
    ck_assert_decimal_eq(decimal_mod(dec1, dec2), control_dec);
}
END_TEST

START_TEST(mod_test_3) {
    decimal dec1 = {0x00000003, 0x00000000, 0x00000000, 0x80000000, 0x0};         // -3
    decimal dec2 = {0x00000002, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 2
    decimal control_dec = {0x00000001, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -1
    ck_assert_decimal_eq(decimal_mod(dec1, dec2), control_dec);
}
END_TEST

START_TEST(mod_test_4) {
    decimal dec1 = {0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 3
    decimal dec2 = {0x00000002, 0x00000000, 0x00000000, 0x80000000, 0x0};         // -2
    decimal control_dec = {0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 1
    ck_assert_decimal_eq(decimal_mod(dec1, dec2), control_dec);
}
END_TEST

START_TEST(mod_test_5) {
    decimal dec1 = {0x00000002, 0x00000000, 0x00000000, 0x80000000, 0x0};         // -2
    decimal dec2 = {0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 3
    decimal control_dec = {0x00000002, 0x00000000, 0x00000000, 0x80000000, 0x0};  // -2
    ck_assert_decimal_eq(decimal_mod(dec1, dec2), control_dec);
}
END_TEST

START_TEST(mod_test_6) {
    decimal dec1 = {0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 3
    decimal dec2 = {0x00000002, 0x00000000, 0x00000000, 0x80000000, 0x0};         // -2
    decimal control_dec = {0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 1
    ck_assert_decimal_eq(decimal_mod(dec1, dec2), control_dec);
}
END_TEST

START_TEST(mod_test_7) {
    decimal dec1 = {0x00000607, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 1543
    decimal dec2 = {0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x0};         // 3
    decimal control_dec = {0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 1
    ck_assert_decimal_eq(decimal_mod(dec1, dec2), control_dec);
}
END_TEST

START_TEST(mod_test_8) {
    decimal dec1 = {0x0000007D, 0x00000000, 0x00000000, 0x00020000, 0x0};         // 1.25
    decimal dec2 = {0x00000087, 0x00000000, 0x00000000, 0x00010000, 0x0};         // 13.5
    decimal control_dec = {0x0000007D, 0x00000000, 0x00000000, 0x00020000, 0x0};  // 1.25
    ck_assert_decimal_eq(decimal_mod(dec1, dec2), control_dec);
}
END_TEST

START_TEST(float_to_dec_1) {
    float x = 1.5;
    decimal test_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    from_float_to_decimal(x, &test_dec);
    decimal control = {0x0000000F, 0x00000000, 0x00000000, 0x00010000, 0x0};  // 1.5
    ck_assert_decimal_eq(test_dec, control);
}
END_TEST

START_TEST(float_to_dec_2) {
    float x = 3442.234;
    decimal test_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    from_float_to_decimal(x, &test_dec);
    decimal control = {0x0034863A, 0x00000000, 0x00000000, 0x00030000, 0x0};  // 3442.234
    ck_assert_decimal_eq(test_dec, control);
}
END_TEST

START_TEST(float_to_dec_4) {
    float x = 1E-28;
    decimal test_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    from_float_to_decimal(x, &test_dec);
    decimal control = {0x00000001, 0x00000000, 0x00000000, 0x001C0000, 0x0};
    // 0.0000000000000000000000000001
    ck_assert_decimal_eq(test_dec, control);
}
END_TEST

START_TEST(float_to_dec_5) {
    float x = -5.333;
    decimal test_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    from_float_to_decimal(x, &test_dec);
    decimal control = {0x000014D5, 0x00000000, 0x00000000, 0x80030000, 0x0};  // -5.333
    ck_assert_decimal_eq(test_dec, control);
}
END_TEST

START_TEST(float_to_dec_6) {
    float x = 0.3333333;
    decimal test_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    from_float_to_decimal(x, &test_dec);
    decimal control = {0x0032DCD5, 0x00000000, 0x00000000, 0x00070000, 0x0};  // 0.3333333
    ck_assert_decimal_eq(test_dec, control);
}
END_TEST

START_TEST(float_to_dec_7) {
    float x = 0.5555556;
    decimal test_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    from_float_to_decimal(x, &test_dec);
    decimal control = {0x0054C564, 0x00000000, 0x00000000, 0x00070000, 0x0};  // 0.5555556
    ck_assert_decimal_eq(test_dec, control);
}
END_TEST

START_TEST(float_to_dec_8) {
    float x = 999999;
    decimal test_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    from_float_to_decimal(x, &test_dec);
    decimal control = {0x000F423F, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 999999
    ck_assert_decimal_eq(test_dec, control);
}
END_TEST

START_TEST(float_to_dec_9) {
    float x = 0;
    decimal test_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    from_float_to_decimal(x, &test_dec);
    decimal control = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    ck_assert_decimal_eq(test_dec, control);
}
END_TEST

START_TEST(float_to_dec_10) {
    float x = 1000000;
    decimal test_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    from_float_to_decimal(x, &test_dec);
    decimal control = {0x000F4240, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 1000000
    ck_assert_decimal_eq(test_dec, control);
}
END_TEST

START_TEST(float_to_dec_11) {
    float x = -1.5E-20;
    decimal test_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    from_float_to_decimal(x, &test_dec);
    decimal control = {0x0000000F, 0x00000000, 0x00000000, 0x80150000, 0x0};  // -0.000000000000000000015
    ck_assert_decimal_eq(test_dec, control);
}
END_TEST

START_TEST(float_to_dec_12) {
    float x = 1.0 / 0.0;                                                           // INF
    decimal test_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    from_float_to_decimal(x, &test_dec);
    decimal control = {0x00000000, 0x00000000, 0x00000000, 0x00000000, D_INFINITY};  // 0
    ck_assert_decimal_eq(test_dec, control);
}
END_TEST

START_TEST(float_to_dec_13) {
    float x = -1.0 / 0.0;                                                          // -INF
    decimal test_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    from_float_to_decimal(x, &test_dec);
    decimal control = {0x00000000, 0x00000000, 0x00000000, 0x00000000, D_NEGATIVE_INFINITY};  // 0
    ck_assert_decimal_eq(test_dec, control);
}
END_TEST

START_TEST(float_to_dec_14) {
    float x = NAN;                                                                 // NAN
    decimal test_dec = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};  // 0
    from_float_to_decimal(x, &test_dec);
    decimal control = {0x00000000, 0x00000000, 0x00000000, 0x00000000, D_NAN};  // 0
    ck_assert_decimal_eq(test_dec, control);
}
END_TEST

START_TEST(float_to_dec_15) {
    float x = 0.0;
    decimal* test_dec = NULL;
    ck_assert_int_eq(from_float_to_decimal(x, test_dec), CONVERTING_ERROR);
}
END_TEST

Suite* decimal_suite(void) {
    Suite* s = suite_create("Decimal");
    TCase* tc_core = tcase_create("Decimal core");

    // Core functions
    tcase_add_test(tc_core, get_bit_test);
    tcase_add_test(tc_core, rm_bit_test);
    tcase_add_test(tc_core, set_bit_test);
    tcase_add_test(tc_core, set_sign_test);
    tcase_add_test(tc_core, rm_sign_test);
    tcase_add_test(tc_core, get_sign_test);
    tcase_add_test(tc_core, get_scale_test);
    tcase_add_test(tc_core, set_scale_test);
    tcase_add_test(tc_core, rm_scale_test);
    tcase_add_test(tc_core, left_shift_test);
    tcase_add_test(tc_core, right_shift_test);
    tcase_add_test(tc_core, dop_code_convert_test);
    tcase_add_test(tc_core, ten_div_test);
    tcase_add_test(tc_core, normalization_test);
    tcase_add_test(tc_core, normalization_test_1);
    tcase_add_test(tc_core, normalization_test_2);
    tcase_add_test(tc_core, overflow_control_test);

    tcase_add_test(tc_core, add_test_1);
    tcase_add_test(tc_core, add_test_2);
    tcase_add_test(tc_core, add_test_3);
    tcase_add_test(tc_core, add_test_4);
    tcase_add_test(tc_core, add_test_5);
    tcase_add_test(tc_core, add_test_6);
    tcase_add_test(tc_core, add_test_7);
    tcase_add_test(tc_core, add_test_8);
    tcase_add_test(tc_core, add_test_9);
    tcase_add_test(tc_core, add_test_10);
    tcase_add_test(tc_core, add_test_11);
    tcase_add_test(tc_core, add_test_12);
    tcase_add_test(tc_core, add_test_13);
    tcase_add_test(tc_core, add_test_14);
    tcase_add_test(tc_core, add_test_15);
    tcase_add_test(tc_core, add_test_16);
    tcase_add_test(tc_core, add_test_17);
    tcase_add_test(tc_core, add_test_18);
    tcase_add_test(tc_core, add_test_19);
    tcase_add_test(tc_core, add_test_20);
    tcase_add_test(tc_core, add_test_21);
    tcase_add_test(tc_core, add_test_22);
    tcase_add_test(tc_core, add_test_23);
    tcase_add_test(tc_core, add_test_24);
    tcase_add_test(tc_core, add_test_25);
    tcase_add_test(tc_core, add_test_26);
    tcase_add_test(tc_core, add_test_27);

    tcase_add_test(tc_core, sub_test_1);
    tcase_add_test(tc_core, sub_test_2);
    tcase_add_test(tc_core, sub_test_3);
    tcase_add_test(tc_core, sub_test_4);
    tcase_add_test(tc_core, sub_test_5);
    tcase_add_test(tc_core, sub_test_6);
    tcase_add_test(tc_core, sub_test_7);
    tcase_add_test(tc_core, sub_test_8);

    tcase_add_test(tc_core, div_test_1);
    tcase_add_test(tc_core, div_test_2);
    tcase_add_test(tc_core, div_test_3);
    tcase_add_test(tc_core, div_test_4);
    tcase_add_test(tc_core, div_test_5);
    tcase_add_test(tc_core, div_test_6);
    tcase_add_test(tc_core, div_test_7);
    tcase_add_test(tc_core, div_test_8);
    tcase_add_test(tc_core, div_test_9);
    tcase_add_test(tc_core, div_test_10);
    tcase_add_test(tc_core, div_test_11);
    tcase_add_test(tc_core, div_test_12);
    tcase_add_test(tc_core, div_test_13);
    tcase_add_test(tc_core, div_test_14);
    tcase_add_test(tc_core, div_test_15);
    tcase_add_test(tc_core, div_test_16);
    tcase_add_test(tc_core, div_test_17);
    tcase_add_test(tc_core, div_test_18);
    tcase_add_test(tc_core, div_test_19);
    tcase_add_test(tc_core, div_test_20);
    tcase_add_test(tc_core, div_test_21);
    tcase_add_test(tc_core, div_test_22);
    tcase_add_test(tc_core, div_test_23);

    tcase_add_test(tc_core, mod_test_1);
    tcase_add_test(tc_core, mod_test_2);
    tcase_add_test(tc_core, mod_test_3);
    tcase_add_test(tc_core, mod_test_4);
    tcase_add_test(tc_core, mod_test_5);
    tcase_add_test(tc_core, mod_test_6);
    tcase_add_test(tc_core, mod_test_7);
    tcase_add_test(tc_core, mod_test_8);

    // Equasion functions
    tcase_add_test(tc_core, less_norm_1);
    tcase_add_test(tc_core, less_norm_2);
    tcase_add_test(tc_core, less_norm_3);
    tcase_add_test(tc_core, less_norm_4);
    tcase_add_test(tc_core, less_NANs);
    tcase_add_test(tc_core, less_INFs);
    tcase_add_test(tc_core, less_1_INF_2_NEG_INF);
    tcase_add_test(tc_core, less_NEG_INFs);

    tcase_add_test(tc_core, equal_norm_1);
    tcase_add_test(tc_core, equal_norm_2);
    tcase_add_test(tc_core, equal_norm_3);
    tcase_add_test(tc_core, equal_norm_4);
    tcase_add_test(tc_core, equal_INFs);
    tcase_add_test(tc_core, equal_NEG_INFs);
    tcase_add_test(tc_core, equal_NANs);

    tcase_add_test(tc_core, not_equal_norm_1);
    tcase_add_test(tc_core, not_equal_norm_2);
    tcase_add_test(tc_core, not_equal_norm_3);
    tcase_add_test(tc_core, not_equal_norm_4);
    tcase_add_test(tc_core, not_equal_INFs);
    tcase_add_test(tc_core, not_equal_NEG_INFs);
    tcase_add_test(tc_core, not_equal_NANs);

    tcase_add_test(tc_core, less_equal_norm_1);
    tcase_add_test(tc_core, less_equal_norm_2);
    tcase_add_test(tc_core, less_equal_norm_3);
    tcase_add_test(tc_core, less_equal_norm_4);
    tcase_add_test(tc_core, less_equal_norm_5);
    tcase_add_test(tc_core, less_equal_INFs);
    tcase_add_test(tc_core, less_equal_NEG_INFs);
    tcase_add_test(tc_core, less_equal_NANs);

    tcase_add_test(tc_core, greater_norm_1);
    tcase_add_test(tc_core, greater_norm_2);
    tcase_add_test(tc_core, greater_norm_3);
    tcase_add_test(tc_core, greater_norm_4);
    tcase_add_test(tc_core, greater_norm_5);
    tcase_add_test(tc_core, greater_norm_6);
    tcase_add_test(tc_core, greater_INFs);
    tcase_add_test(tc_core, greater_NEG_INFs);
    tcase_add_test(tc_core, greater_NANs);
    tcase_add_test(tc_core, greater_1_INF_2_NEG_INF);

    tcase_add_test(tc_core, greater_equal_norm_1);
    tcase_add_test(tc_core, greater_equal_norm_2);
    tcase_add_test(tc_core, greater_equal_norm_3);
    tcase_add_test(tc_core, greater_equal_norm_4);
    tcase_add_test(tc_core, greater_equal_INFs);
    tcase_add_test(tc_core, greater_equal_NEG_INFs);
    tcase_add_test(tc_core, greater_equal_NANs);
    tcase_add_test(tc_core, greater_equal_1_INF_2_NEG_INF);

    // Arithmetic tests
    tcase_add_test(tc_core, mul_norm_1);
    tcase_add_test(tc_core, mul_norm_2);
    tcase_add_test(tc_core, mul_norm_3);
    tcase_add_test(tc_core, mul_norm_4);
    tcase_add_test(tc_core, mul_overflow);
    // Converters
    tcase_add_test(tc_core, from_int_to_decimal_test);

    tcase_add_test(tc_core, from_decimal_to_int_1);
    tcase_add_test(tc_core, from_decimal_to_int_2);
    tcase_add_test(tc_core, from_decimal_to_int_3);
    tcase_add_test(tc_core, from_decimal_to_int_4);
    tcase_add_test(tc_core, from_decimal_to_int_5);
    tcase_add_test(tc_core, from_decimal_to_int_6);
    tcase_add_test(tc_core, from_decimal_to_int_7);
    tcase_add_test(tc_core, from_decimal_to_int_8);
    tcase_add_test(tc_core, from_decimal_to_int_9);

    tcase_add_test(tc_core, float_to_dec_1);
    tcase_add_test(tc_core, float_to_dec_2);
    tcase_add_test(tc_core, float_to_dec_4);
    tcase_add_test(tc_core, float_to_dec_5);
    tcase_add_test(tc_core, float_to_dec_6);
    tcase_add_test(tc_core, float_to_dec_7);
    tcase_add_test(tc_core, float_to_dec_8);
    tcase_add_test(tc_core, float_to_dec_9);
    tcase_add_test(tc_core, float_to_dec_10);
    tcase_add_test(tc_core, float_to_dec_11);
    tcase_add_test(tc_core, float_to_dec_12);
    tcase_add_test(tc_core, float_to_dec_13);
    tcase_add_test(tc_core, float_to_dec_14);
    tcase_add_test(tc_core, float_to_dec_15);

    // Others
    tcase_add_test(tc_core, truncate_test_1);
    tcase_add_test(tc_core, truncate_test_2);
    tcase_add_test(tc_core, truncate_test_3);
    tcase_add_test(tc_core, truncate_test_4);

    tcase_add_test(tc_core, floor_test_1);
    tcase_add_test(tc_core, floor_test_2);
    tcase_add_test(tc_core, floor_test_3);
    tcase_add_test(tc_core, floor_test_4);
    tcase_add_test(tc_core, floor_test_5);

    tcase_add_test(tc_core, round_test_1);
    tcase_add_test(tc_core, round_test_2);
    tcase_add_test(tc_core, round_test_3);
    tcase_add_test(tc_core, round_test_4);
    tcase_add_test(tc_core, round_test_5);
    tcase_add_test(tc_core, round_test_6);
    tcase_add_test(tc_core, round_test_7);
    tcase_add_test(tc_core, round_test_8);
    tcase_add_test(tc_core, round_test_9);
    tcase_add_test(tc_core, round_test_10);
    tcase_add_test(tc_core, round_test_11);
    tcase_add_test(tc_core, round_test_12);
    tcase_add_test(tc_core, round_test_13);
    tcase_add_test(tc_core, round_test_14);
    tcase_add_test(tc_core, round_test_15);
    tcase_add_test(tc_core, round_test_16);
    tcase_add_test(tc_core, round_test_17);
    tcase_add_test(tc_core, round_test_18);
    tcase_add_test(tc_core, round_test_19);
    tcase_add_test(tc_core, round_test_20);
    tcase_add_test(tc_core, round_test_21);
    tcase_add_test(tc_core, round_test_22);
    tcase_add_test(tc_core, round_test_23);
    tcase_add_test(tc_core, round_test_24);

    suite_add_tcase(s, tc_core);
    return s;
}

int main() {
    int number_failed;
    Suite* s = decimal_suite();
    SRunner* sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
