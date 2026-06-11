#include <stdio.h>

#include "number_formatter.h"

/*
 * Prevent buffer overflow
 * Validate base range (2–36 only)
 * Handle invalid menu choices
 * Handle empty input
 * Convert from base X → decimal
 * Convert from base X → base Y
 * Support unsigned numbers
 * Handle extremely large numbers using strings
 * Format currency style
 * Format scientific notation
 * Accept large integers
 * Handle negative numbers
 * Insert commas correctly
 * Convert to binary
 * Convert to octal
 * Convert to hex
 * Convert to custom base (2–36)
 * Use multiple functions
 * Be modular and readable
 */

// declarations
int get_extra_format_char_count(int);
char* get_formatted_integer_string(lli, char*, int);
int get_num_of_digits_in_integer(lli);

// definitions

void format_lli(lli number) {
  // 1,000 1,00,000 -24,000 13,245.45
  printf("original number: %lld\n", number);

  int length = get_num_of_digits_in_integer(number);
  char formatted_number[length];
  get_formatted_integer_string(number, formatted_number, length);
  printf("%s\n", formatted_number);
}

void format_double(double number) {
  printf("in format_double func: %lf\n", number);
  // get the int part.
  long long int integer_part = number;
  // get the double part.
  double double_part = number - integer_part;

  int length = get_num_of_digits_in_integer(number);
  char formatted_number[length];
  get_formatted_integer_string(number, formatted_number, length);

  printf("%lf\n", double_part);
}

char* get_formatted_integer_string(lli num, char* formatted_number, int length) {
  lli tmp_num = num;
  printf("tmp num: %lld length: %d\n", tmp_num, length);
  int format_char_index = 4; // for integers: ,xyz'\0'
  for (int i = length; i >= 0; i--) {
    printf("i: %d\n", i);
    if (i == length) {
      formatted_number[i] = '\0';
      continue;
    }
    if (i == length - format_char_index) {
      formatted_number[i] = FORMAT_CHAR;
      format_char_index += 3;
    } else {
      printf("%dth iter: %lld\n", i, tmp_num % 10);
      formatted_number[i] = (tmp_num % 10) + ASCII_ZERO_DEC;
      tmp_num /= 10;
    }
  }

  return formatted_number;
}

int get_num_of_digits_in_integer(lli num) {
  // find number of digits
  lli tmp_num = num;
  int digits = 0;
  while (tmp_num > 0) {
    digits++;
    tmp_num /= 10;
  }
  printf("digits: %d\n", digits);

  int extra_chars = get_extra_format_char_count(digits);
  printf("extra_chars: %d\n", extra_chars);

  int length = digits + extra_chars; // 0 to length
  return length;
}

int get_extra_format_char_count(int digits) {
  // 9-0
  // 99-0
  // 999-0
  // 9,999-1 - 4
  // 99,999-1 - 5
  // 9,99,999-2 - 6
  // 99,99,999-2 - 7
  // 9,99,99,999-3 - 8
  // 99,99,99,999-3 - 9
  // 9,99,99,99,999-4 - 10

  if (digits <= 3) {
    return 0;
  }

  if (digits % 2 != 0) {
    digits -= 1;
  }

  return (digits / 2) - 1;
}
