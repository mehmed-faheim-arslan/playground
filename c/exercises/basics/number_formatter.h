#ifndef NUMBER_FORMATTER_H_
#define NUMBER_FORMATTER_H_

#define FORMAT_CHAR ','
#define ASCII_ZERO_DEC 48

typedef long long int lli;

char* format_lli(char*);
char* format_double(char*);

char* convert(lli, char*, int);

#endif // NUMBER_FORMATTER_H_
