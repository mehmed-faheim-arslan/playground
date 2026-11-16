#ifndef NUMBER_FORMATTER_H_
#define NUMBER_FORMATTER_H_

#include <stddef.h> /* size_t */

/*
 * number_formatter
 * ----------------
 * Insert thousands separators into a number that is given as TEXT.
 *
 * Why text and not `long long` / `double`?
 *   - A string has no value limit, so "handle extremely large numbers"
 *     comes for free.
 *   - A `double` cannot store most decimal fractions exactly (0.1 is really
 *     0.1000000000000000055...). Formatting the digits the user actually
 *     typed avoids that whole class of rounding bugs: we never do float math.
 *
 * Both functions follow the same contract:
 *   - `in`     : the source string, NUL-terminated.
 *   - `out`    : caller-owned buffer the result is written into.
 *   - `out_sz` : how many bytes `out` can hold (including the NUL).
 *   - return   : `out` on success, or NULL if the input is malformed or
 *                `out` is too small. On NULL the contents of `out` are
 *                unspecified.
 *
 * The caller owns all memory. These functions never allocate and never print.
 */

/* The character inserted between digit groups. */
#define FORMAT_CHAR ','

/* "-1234567"  -> "-1,234,567"
 * Accepts an optional leading '-' followed by one or more decimal digits.
 * Anything else (empty, "+5", "12a", "1.5") is rejected with NULL.
 */
char *format_int_str(const char *in, char *out, size_t out_sz);

/* "-1234567.45" -> "-1,234,567.45"
 * Groups the integer part; copies the '.' and everything after it verbatim.
 * With no '.', behaves exactly like format_int_str.
 */
char *format_decimal_str(const char *in, char *out, size_t out_sz);

#endif /* NUMBER_FORMATTER_H_ */
