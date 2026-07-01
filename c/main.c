#include "exercises/basics/number_formatter.h"

typedef long long int lli;

int main() {
  lli num_lli = 5536669205;
  double num_double = 1234.342342;
  FORMAT(num_lli);
  FORMAT(num_double);

  return 0;
}
