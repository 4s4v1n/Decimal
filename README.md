# Decimal

![10 power](./images/Visualisation_billion.png)

## A project is currently proposed to implement the data type from C# decimal, in C, using bitwise operations

The concept of a decimal number is implemented through a structure that stores an array of 4 elements of the `int` type and one element of the `value_type_t` type, indicating what kind of value it is.

## Structure decimal

 1) The first three elements of the bits array (96 bits) store the normalized form of the number.
 2) The fourth element of the bits array stores the characteristics of the number, namely the sign at the 31st bit position and the exponent.
 3) `value_type_t` - indicates whether it is a normal number (D_NORMAL_VALUE), nan (D_NAN), infinity (D_INFINITY), -infinity(D_NEGATIVE_INFINITY) or a number converted to two's complement code (D_DOP_CODE).
 4) This project defines `TRUE` - 0, and `FALSE` - 1.

The decimal number itself is calculated using the formula: `(normalized form)*(-10^(exponent))` or `N*(-10^(M))`.

## Functions for decimal

The main functions for working with numbers of the decimal type are divided into 4 logical blocks: arithmetic, comparisons, converters and special functions.

### Arithmetic

 1) `decimal decimal_add(decimal x, decimal y)` - returns x + y .
 2) `decimal decimal_sub(decimal x, decimal y)` - returns x - y .
 3) `decimal decimal_mul(decimal x, decimal y)` - returns x * y .
 4) `decimal decimal_div(decimal x, decimal y)` - returns x / y .
 5) `decimal decimal_mod(decimal x, decimal y)` - returns x mod y (remainder of division) .

### Comparisons

 1) `int is_less(decimal x, decimal y)` - returns 0 if x < y, otherwise 1.
 2) `int is_less_or_equal(decimal x, decimal y)` - returns 0 if x <= y, otherwise 1.
 3) `int is_greater(decimal x, decimal y)` - returns 0 if x > y, otherwise 1.
 4) `int is_greater_or_equal(decimal x, decimal y)` - returns 0 if x >= y, otherwise 1.
 5) `int is_equal(decimal x, decimal y)` - returns 0 if x == y, otherwise 1.
 6) `int is_not_equal(decimal x, decimal y)` - returns 0 if x != y, otherwise 1.

### Converters

 1) `int from_int_to_decimal(int src, decimal* dst)` - converts the passed int number to decimal and returns 0 if the conversion was successful, 1 if it failed.
 2) `int from_float_to_decimal(float src, decimal* dst)` - converts the given float number to decimal and returns 0 if the conversion was successful, 1 if it failed.
 3) `int from_decimal_to_int(decimal src, int* dst)` - converts the given decimal number to int and returns 0 in case of successful conversion, 1 in case of failure.
 4) `int from_decimal_to_float(decimal src, float* dst)` - converts the passed decimal number to float and returns 0 in case of successful conversion, 1 in case of failure.

### Special functions

 1) `decimal decimal_floor(decimal dec)` - returns the result of rounding dec to the nearest integer less than or equal to dec.
 2) `decimal decimal_round(decimal dec)` - - returns the result of rounding dec to the nearest whole number.
 3) `decimal decimal_truncate(decimal dec)` - returns the integer part of the given dec.
 4) `decimal decimal_negate(decimal dec)` - returns dec times -1.

## Examples

 1) `decimal dec = {0x74ED9B81, 0xE758E637, 0x00000019, 0x00060000, 0x0}` is 477838928989310.000001 .
 2) `decimal dec = {0x0, 0x0, 0x0, 0x0, 0x1}` is inf.
 3) `decimal dec = {0x0, 0x0, 0x0, 0x0, 0x2}` is -inf.
 4) `decimal dec = {0x0, 0x0, 0x0, 0x0, 0x3}` is nan.
 5) `decimal dec = {0x000007F2, 0x00000000, 0x00000000, 0x80080000, 0x0}` is -0.00002034 .
 6) `decimal dec = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0, 0x0}` is 79228162514264337593543950335 .
 7) `decimal dec = {0x00000001, 0x00000000, 0x00000000, 0x001C0000, 0x0}` is 1e-28.


The assembly of the project is carried out at your discretion. In my case, building with gcc 9.2.0 and a Makefile on Linux.

Makefile, like all source materials, is in the src folder.