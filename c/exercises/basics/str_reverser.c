#include "str_reverser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse(const char *str) {
  int len = strlen(str);
  char *op = malloc(len + 1);
  for (int i = 0; i < len; i++) {
    op[i] = str[len - i - 1];
  }
  op[len] = '\0';
  printf("%s\n", op);
}
