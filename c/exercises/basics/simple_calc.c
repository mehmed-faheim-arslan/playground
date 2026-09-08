#include "simple_calc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *get_error_code(ErrorCode error) {
  switch (error) {
  case SUCCESS:
    return "operation successful";
  case DIVIDE_BY_ZERO:
    return "error: divide by zero attempted";
  case INVALID_OPERATOR:
    return "invalid operator symbol used.";
  case INVALID_ARG_COUNT:
    return "invalid argument count";
  case INVALID_MOD_OPERAND:
    return "modulus operation allowed only with integers";
  case INVALID_OPERAND:
    return "invalid operand format/text";
  default:
    return "unknown error code";
  }
}

double calculate(double num1, double num2, char operator) {
  switch (operator) {
  case '+':
    return num1 + num2;
  case '-':
    return num1 - num2;
  case '*':
    return num1 * num2;
  case '/':
    return num1 / num2;
  default:
    return -1;
  }
}

void check_input(int argc, char **argv) {
  char *endptr = NULL;
  if (argc != 4) {
    printf("%s\n", get_error_code(INVALID_ARG_COUNT));
    return;
  }
  if (strcmp(argv[2], "+") != 0 && strcmp(argv[2], "-") != 0 &&
      strcmp(argv[2], "*") != 0 && strcmp(argv[2], "/") != 0) {
    printf("%s\n", get_error_code(INVALID_OPERATOR));
    return;
  }
  long a, b;
  if (parse_int(argv[1], &a) || parse_int(argv[3], &b)) {
    printf("%s\n", get_error_code(INVALID_OPERAND));
    return;
  }
  if (strcmp(argv[2], "/") == 0 && argv[3] == 0) {
    printf("%s\n", get_error_code(DIVIDE_BY_ZERO));
    return;
  }

  printf("%g\n", calculate(a, b, argv[2][0]));
}

int parse_int(const char *s, long *out) {
  char *end;
  errno = 0;
  long val = strtol(s, &end, 10);
  if (end == s)
    return -1;
  if (*end != '\0')
    return -1;
  if (errno == ERANGE)
    return -1;

  *out = val;
  return 0;
}
