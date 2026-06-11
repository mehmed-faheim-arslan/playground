#ifndef NUMBER_FORMATTER_H_
#define NUMBER_FORMATTER_H_

#define FORMAT_CHAR ','
#define ASCII_ZERO_DEC 48

typedef long long int lli;

#define FORMAT(N)           \
  _Generic( (N),            \
    int    : format_lli,    \
    long   : format_lli,    \
    lli    : format_lli,    \
    float  : format_double, \
    double : format_double  \
  )( (N) )

void format_lli(lli number);
void format_double(double number);

#endif // NUMBER_FORMATTER_H_
