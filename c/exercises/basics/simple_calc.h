#ifndef SIMPLE_CALC_H
#define SIMPLE_CALC_H

#include <stdio.h>

typedef enum {
  SUCCESS = 0,
  DIVIDE_BY_ZERO,
  INVALID_OPERATOR,
  INVALID_OPERAND,
  INVALID_ARG_COUNT,
  INVALID_MOD_OPERAND
} ErrorCode;

const char *get_error_code(ErrorCode error);

double calculate(double, double, char);
void check_input(int, char **);
int parse_int(const char *s, long *out);

#endif
