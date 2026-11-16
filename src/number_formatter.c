#include "number_formatter.h"

#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------------------- *
 *  format_int_str
 * ------------------------------------------------------------------------- *
 *
 *  Goal: turn a plain digit string into a comma-grouped one.
 *
 *      "1"          -> "1"
 *      "1234"       -> "1,234"
 *      "1000000"    -> "1,000,000"
 *      "-98765"     -> "-98,765"
 *
 *  Strategy: we know exactly how big the answer is before we build it, so we
 *  allocate nothing and fill the caller's buffer from the RIGHT (least
 *  significant digit) towards the LEFT. Filling right-to-left is what makes
 *  the grouping trivial: commas go after every 3rd digit counted from the
 *  end, and "from the end" is just "every 3rd step of our loop".
 */
char *format_int_str(const char *in, char *out, size_t out_sz)
{
    /* ---- 1. reject obviously bad arguments -------------------------------
     * A NULL string or NULL buffer is a programming error, not a value we
     * can format. Bail immediately.
     */
    if (in == NULL || out == NULL)
        return NULL;

    /* ---- 2. peel off an optional leading minus sign --------------------
     * `neg` remembers whether we saw one. `d` then points at the first
     * actual digit, so the rest of the function only ever deals with
     * digits and can ignore the sign until the very end.
     */
    bool neg = (in[0] == '-');
    const char *d = in + (neg ? 1 : 0);

    /* ---- 3. validate: must be 1+ characters, all of them digits -------- */
    size_t n = strlen(d);            /* number of digits in the input      */
    if (n == 0)                      /* ""  or  "-"  -> nothing to format   */
        return NULL;
    for (size_t i = 0; i < n; i++)
        if (d[i] < '0' || d[i] > '9')   /* "12a", "1.5", "+3" -> rejected  */
            return NULL;

    /* ---- 4. work out the exact size of the result --------------------
     *
     *   commas: one comma per full group of 3 digits *boundary*.
     *           3 digits  -> 0 commas
     *           4 digits  -> 1 comma        (1,234)
     *           6 digits  -> 1 comma        (123,456)
     *           7 digits  -> 2 commas       (1,234,567)
     *           => (n - 1) / 3     (integer division)
     *
     *   need = optional '-'  +  n digits  +  commas  +  1 for the '\0'
     */
    size_t commas = (n - 1) / 3;
    size_t need   = (neg ? 1u : 0u) + n + commas + 1u;

    if (out_sz < need)              /* caller's buffer can't hold it       */
        return NULL;

    /* ---- 5. fill the buffer back-to-front ---------------------------
     *
     *   `w` is the WRITE cursor. It starts just past the last real
     *   character (where the '\0' goes) and moves left with `--w`.
     *
     *   `since` counts how many digits we've written since the last
     *   comma (or since the start). Every time it reaches 3 we drop a
     *   comma -- unless there are no digits left to its left (`r == 0`),
     *   because "1,234" must not come out as ",1,234".
     *
     *   `r` walks the input digits from the end: the loop header
     *   `for (r = n; r-- > 0; )` visits r = n-1, n-2, ... , 0.
     */
    size_t w = need - 1;           /* index of the terminator slot         */
    out[w] = '\0';

    size_t since = 0;
    for (size_t r = n; r-- > 0; ) {
        out[--w] = d[r];          /* copy one digit, moving left           */
        since++;

        if (since == 3 && r != 0) {   /* group full AND more digits remain */
            out[--w] = FORMAT_CHAR;
            since = 0;
        }
    }

    /* ---- 6. re-attach the sign, if any ------------------------------
     * After the loop `w` sits at index 1 (if negative) or 0. The one
     * remaining slot is exactly where '-' belongs.
     */
    if (neg)
        out[--w] = '-';

    /* `w` is now 0 and `out` holds the finished, NUL-terminated string. */
    return out;
}

/* ------------------------------------------------------------------------- *
 *  format_decimal_str
 * ------------------------------------------------------------------------- *
 *
 *  Goal: same grouping, but only on the part left of the decimal point.
 *
 *      "1234.5"      -> "1,234.5"
 *      "1234567.899" -> "1,234,567.899"
 *      "-1000.00"    -> "-1,000.00"
 *      "1234"        -> "1,234"          (no dot: just defer to the int path)
 *
 *  The fractional part is copied character-for-character. We do NOT parse it
 *  as a number, so "1234.5000" keeps its trailing zeros and no rounding can
 *  creep in.
 */
char *format_decimal_str(const char *in, char *out, size_t out_sz)
{
    if (in == NULL || out == NULL)
        return NULL;

    /* Find the decimal point. strchr returns NULL if there isn't one. */
    const char *dot = strchr(in, '.');
    if (dot == NULL)
        return format_int_str(in, out, out_sz);   /* plain integer */

    /* ---- 1. copy the integer part into a small scratch buffer -------
     * `ilen` is the number of characters before the '.'. We need it as
     * its own NUL-terminated string to hand to format_int_str.
     *
     * 64 bytes covers a 63-digit integer part (including a sign) -- far
     * more than any real use; if somehow exceeded we fail cleanly rather
     * than overflow the scratch buffer.
     */
    char intpart[64];
    size_t ilen = (size_t)(dot - in);   /* pointer difference = length    */
    if (ilen >= sizeof intpart)
        return NULL;

    memcpy(intpart, in, ilen);
    intpart[ilen] = '\0';

    /* ---- 2. group the integer part straight into the caller's buffer -- */
    if (format_int_str(intpart, out, out_sz) == NULL)
        return NULL;                     /* bad digits, or out too small   */

    /* ---- 3. append the fractional part verbatim --------------------
     *   `used` : characters already in `out` (the grouped integer part).
     *   `frac` : length of ".899" INCLUDING the dot, via strlen(dot).
     *   We need room for `used + frac` characters plus one '\0'.
     */
    size_t used = strlen(out);
    size_t frac = strlen(dot);
    if (used + frac + 1 > out_sz)
        return NULL;

    /* Copy frac + 1 bytes so the source string's own '\0' comes along. */
    memcpy(out + used, dot, frac + 1);

    return out;
}

/* ------------------------------------------------------------------------- *
 *  Self-test.  Build with:  gcc -DNF_TEST number_formatter.c -o nf && ./nf
 *  Silent success, aborts on the first wrong result.
 * ------------------------------------------------------------------------- */
#ifdef NF_TEST
#include <assert.h>
#include <stdio.h>

static void eq(char *(*fn)(const char *, char *, size_t),
               const char *in, const char *want)
{
    char buf[64];
    char *r = fn(in, buf, sizeof buf);
    assert(r != NULL);
    if (strcmp(r, want) != 0) {
        fprintf(stderr, "FAIL  in=%-16s got=%-16s want=%s\n", in, r, want);
        assert(0);
    }
}

static void is_null(char *(*fn)(const char *, char *, size_t), const char *in)
{
    char buf[64];
    assert(fn(in, buf, sizeof buf) == NULL);
}

int main(void)
{
    /* integers */
    eq(format_int_str, "0",           "0");
    eq(format_int_str, "5",           "5");
    eq(format_int_str, "42",          "42");
    eq(format_int_str, "123",         "123");
    eq(format_int_str, "1234",        "1,234");
    eq(format_int_str, "12345",       "12,345");
    eq(format_int_str, "123456",      "123,456");
    eq(format_int_str, "1234567",     "1,234,567");
    eq(format_int_str, "1000000",     "1,000,000");
    eq(format_int_str, "-1",          "-1");
    eq(format_int_str, "-1234",       "-1,234");
    eq(format_int_str, "-1000000",    "-1,000,000");

    /* decimals */
    eq(format_decimal_str, "1234.5",       "1,234.5");
    eq(format_decimal_str, "1234567.899",  "1,234,567.899");
    eq(format_decimal_str, "-1000.00",     "-1,000.00");
    eq(format_decimal_str, "12.3456",      "12.3456");
    eq(format_decimal_str, "1234",         "1,234");   /* no dot */
    eq(format_decimal_str, "0.1",          "0.1");     /* no float math */

    /* rejects */
    is_null(format_int_str, "");
    is_null(format_int_str, "-");
    is_null(format_int_str, "+5");
    is_null(format_int_str, "12a");
    is_null(format_int_str, "1.5");     /* has a dot -> not an integer */

    /* buffer too small: "1,234,567" needs 10 bytes incl NUL */
    {
        char tiny[9];
        assert(format_int_str("1234567", tiny, sizeof tiny) == NULL);
    }

    puts("all number_formatter tests passed");
    return 0;
}
#endif /* NF_TEST */
