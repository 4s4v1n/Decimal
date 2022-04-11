#ifndef SRC_DECIMAL_H_
#define SRC_DECIMAL_H_

#include <math.h>
#include <stdio.h>

typedef enum value_type {
  D_NORMAL_VALUE = 0,
  D_INFINITY = 1,
  D_NEGATIVE_INFINITY = 2,
  D_NAN = 3,
  D_DOP_CODE = 4
} value_type_t;

typedef struct {
  int bits[4];
  value_type_t value_type;
} decimal;

typedef struct {
  int bits[7];
  value_type_t value_type;
} big_decimal;

typedef enum { LOW = 0, MID = 1, HIGH = 2, SCALE = 3 } rank;

typedef enum {
  B_LOW = 0,
  B_MID = 1,
  B_HIGH = 2,
  B_UP_LOW = 3,
  B_UP_MID = 4,
  B_UP_HIGH = 5,
  B_SCALE = 6
} b_rank;

typedef enum { SUCCESS = 0, CONVERTING_ERROR = 1 } convert;

typedef enum { TRUE = 0, FALSE = 1 } compare;

// Основные функции
// --------------------------------------------------------------------
decimal decimal_add(decimal x, decimal y);
decimal decimal_sub(decimal x, decimal y);
decimal decimal_mul(decimal x, decimal y);
decimal decimal_div(decimal x, decimal y);
decimal decimal_mod(decimal x, decimal y);

int is_less(decimal x, decimal y);
int is_less_or_equal(decimal x, decimal y);
int is_greater(decimal x, decimal y);
int is_greater_or_equal(decimal x, decimal y);
int is_equal(decimal x, decimal y);
int is_not_equal(decimal x, decimal y);

int from_int_to_decimal(int src, decimal* dst);
int from_float_to_decimal(float src, decimal* dst);
int from_decimal_to_int(decimal src, int* dst);
int from_decimal_to_float(decimal src, float* dst);

decimal decimal_floor(decimal dec);
decimal decimal_round(decimal dec);
decimal decimal_truncate(decimal dec);
decimal decimal_negate(decimal x);
// --------------------------------------------------------------------

// Вспомогательные функции
// --------------------------------------------------------------------

// val - decimal число, i - индекс, rank - "байт"
int get_bit(decimal val, int i);      // узнать знач бита
decimal set_bit(decimal val, int i);  // установить знач бита 1
decimal rm_bit(decimal val, int i);  // установить знач бита 0

int get_big_bit(big_decimal val, int i);
big_decimal set_big_bit(big_decimal val, int i);

int get_sign(decimal val);  // узнать знач бита знака
decimal set_sign(decimal val);  // установить знач бита знака 1
decimal rm_sign(decimal val);  // установить знача бита знака 0

int get_big_sign(big_decimal val);
big_decimal set_big_sign(big_decimal val);
big_decimal rm_big_sign(big_decimal val);

int get_scale(decimal val);               // получение масштаба
void set_scale(decimal* val, int scale);  // установка масштаба
void rm_scale(decimal* val);              // удаление масштаба

int get_big_scale(big_decimal val);
void set_big_scale(big_decimal* val, int scale);
void rm_big_scale(big_decimal* val);

int first_different_bit(decimal x, decimal y);
int first_different_big_bit(big_decimal x, big_decimal y);
int first_big_bit(big_decimal val);  // ищет первую 1 в числе

void init_dec(decimal* dec);
void init_big_dec(big_decimal* big_dec);

void show_float(float val);      // вывод float в двоичной
void show_int(int val);          // вывод int в двоичной
void show_decimal(decimal dec);  // вывод decimal в двоичной
void print_decimal_in_hex(decimal dec);
void print_big_decimal_in_hex(big_decimal dec);
void show_big_decimal(big_decimal big_dec);
void init_big_dec(big_decimal* big_dec);
decimal dec_abs(decimal x);

void left_shift_decimal(decimal* val, int shift);
void right_shift_decimal(decimal* val, int shift);

void left_shift_big_decimal(big_decimal* val, int shift);
void right_shift_big_decimal(big_decimal* val, int shift);

int get_bin_exp(float val);  // получение экспоненты у float

void ten_mult(decimal* val);
void ten_big_mult(big_decimal* val);
decimal ten_div(decimal dec);
decimal ten_div_mod_core(decimal dec, int* mod);
decimal div_mod_core(decimal x, decimal y, decimal* mod);
big_decimal ten_big_div_mod_core(big_decimal dec, int* mod);
void bank_round(big_decimal* val, int mod);
// на сколько вылезло за границы decimal при домножении на 10

// сумма нормализированного числа
decimal sum_wo_scale(decimal x, decimal y);
decimal simple_subtraction(decimal x, decimal y);
int comparer(decimal x, decimal y);

big_decimal sum_big_wo_scale(big_decimal x, big_decimal y);

void normalize(decimal* x, decimal* y);
decimal normalize_rounding(decimal x);
int overflow_control(decimal dec);

void convert_dec_to_big(decimal dec, big_decimal* big_dec);
void convert_big_to_dec(big_decimal big_dec, decimal* dec);

decimal dop_code(decimal dec);
big_decimal big_dop_code(big_decimal dec);
int div_normalize(big_decimal* big_x, big_decimal* big_y);

int get_float_bit(float val, int i);
int incorrect_values_div(decimal x, decimal y, decimal* res);
int incorrect_values_add(decimal x, decimal y, decimal* res);

// --------------------------------------------------------------------

#endif  // SRC_DECIMAL_H_
