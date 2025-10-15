#include "../include/ft_maki.h"
// #include "../include/ft_vfprintf.h"
/*
 * ft_printf
 * Technical Decision: Centralizing Internal Utility Functions

 *
 * To improve performance and compilation efficiency, all utility
 * functions that are only used internally by ft_printf (such as
 * custom memset, memcpy, and floating-point bit operations) are
 * defined as `static inline` within the ft_printf module.
 *
 * Rationale:
 *  1. Inlining and Performance** — These functions are small
 *     (tight loops or simple bit manipulations). Inlining avoids
 *     function call overhead and allows the compiler to perform
 *     cross-function optimizations.
 *
 *  2. Module Self-Containment** — Keeping internal helpers inside
 *     the ft_printf compilation unit reduces external dependencies
 *     and allows ft_printf to be compiled as a standalone component
 *     without linking against the global utility library.
 *
 *  3. Namespace Isolation** — Marking these functions as `static`
 *     prevents symbol collisions and keeps the global namespace clean.
 *
 *  4. Maintainability** — Only shared or public functions (e.g.,
 *     ft_isinf, ft_isnan) remain in ft_utils.c. Internal helpers
 *     specific to printf behavior stay local for better encapsulation.
 *
 * Summary:
 *    Small, high-frequency internal functions → `static inline` here.
 *    Shared or public functions → centralized in ft_utils.
 */

/* internal global variable that indicates which fd the functions write to*/
static int ft_current_fd = 1;

#define FT_PRINTF_BUFSIZE 4096

static char ft_buf[FT_PRINTF_BUFSIZE];
static int ft_buf_len = 0;
static int ft_buf_fd = 1;

static inline void ft_buf_set_fd(int fd) { ft_buf_fd = fd; }

static inline void ft_buf_flush(void) {
  if (ft_buf_len > 0) {
    ft_write(ft_buf_fd, ft_buf, ft_buf_len);
    ft_buf_len = 0;
  }
}

static inline void ft_buf_putchar(char c) {
  if (ft_buf_len >= FT_PRINTF_BUFSIZE)
    ft_buf_flush();
  ft_buf[ft_buf_len++] = c;
}

static inline void ft_buf_putnchar(char c, int n) {
  while (n-- > 0)
    ft_buf_putchar(c);
}

static inline void ft_buf_putstr(const char *s, int len) {
  for (int i = 0; i < len; i++)
    ft_buf_putchar(s[i]);
}

// write using buffer
static inline int ft_printf_pad(char c, int count) {
  if (count <= 0)
    return (0);
  ft_buf_putnchar(c, count);
  return (count);
}

/* fast unsigned numlen for up to 64-bit numbers */
static inline int ft_printf_unumlen_u64(unsigned long long n) {
  if (n < 10ULL)
    return (1);
  if (n < 100ULL)
    return (2);
  if (n < 1000ULL)
    return (3);
  if (n < 10000ULL)
    return (4);
  if (n < 100000ULL)
    return (5);
  if (n < 1000000ULL)
    return (6);
  if (n < 10000000ULL)
    return (7);
  if (n < 100000000ULL)
    return (8);
  if (n < 1000000000ULL)
    return (9);
  if (n < 10000000000ULL)
    return (10);
  if (n < 100000000000ULL)
    return (11);
  if (n < 1000000000000ULL)
    return (12);
  if (n < 10000000000000ULL)
    return (13);
  if (n < 100000000000000ULL)
    return (14);
  if (n < 1000000000000000ULL)
    return (15);
  if (n < 10000000000000000ULL)
    return (16);
  if (n < 100000000000000000ULL)
    return (17);
  if (n < 1000000000000000000ULL)
    return (18);
  if (n < 10000000000000000000ULL)
    return (19);
  return (20);
}

static inline int ft_printf_unumlen(unsigned long n) {
  return (ft_printf_unumlen_u64((unsigned long long)n));
}

static inline int ft_printf_max(int a, int b) { return ((a > b) ? a : b); }

static inline int ft_printf_putchar(char c) {
  ft_buf_putchar(c);
  return (1);
}

static inline int ft_printf_putnchar(char c, int n) {
  ft_buf_putnchar(c, n);
  return (n);
}

static inline int ft_printf_putchar_base(char c, int width, int left) {
  int printed = 0;

  if (!left)
    for (int i = 0; i < width - 1; i++) {
      ft_buf_putchar(' '); // modified
      printed++;           // modified
    }
  ft_buf_putnchar(c, 1); // modified
  printed++;             // modified

  if (left)
    for (int i = 0; i < width - 1; i++) {
      ft_buf_putchar(' '); // modified
      printed++;           // modified
    }
  return (printed);
}

static inline int ft_printf_putstr(char *s, t_flags flags) {
  int len = 0;
  if (!s)
    s = "(null)";
  int str_len = 0;
  while (s[str_len] &&
         (!flags.precision_specified || str_len < flags.precision))
    str_len++;
  if (!flags.left_align)
    len += ft_printf_pad(' ', flags.width - str_len);
  ft_buf_putstr(s, str_len); // modified
  len += str_len;            // modified
  if (flags.left_align)
    len += ft_printf_pad(' ', flags.width - str_len);
  return (len);
}

static inline int ft_printf_putnbr(long long n, t_flags flags) {
  int len = 0;
  unsigned long long tmp =
      (n < 0) ? (unsigned long long)(-n) : (unsigned long long)n;

  int num_len = ft_printf_unumlen_u64(tmp);
  int prec_zeros =
      (flags.precision > num_len) ? (flags.precision - num_len) : 0;
  int total_width = num_len + prec_zeros + (n < 0 ? 1 : 0);
  int pad_spaces =
      (flags.width > total_width) ? (flags.width - total_width) : 0;

  /* left padding (spaces) */
  if (!flags.left_align && !flags.zero_padding)
    len += ft_printf_pad(' ', pad_spaces);

  /* sign */
  if (n < 0) {
    ft_buf_putchar('-');
    len++;
  }

  /* zero padding due to '0' flag (width) */
  if (!flags.left_align && flags.zero_padding)
    len += ft_printf_pad('0', pad_spaces);

  /* precision zeros */
  len += ft_printf_pad('0', prec_zeros);

  /* write digits into local buffer (no recursion) */
  char buf[32];
  int i = 31;
  buf[i--] = '\0';
  if (tmp == 0) {
    buf[i--] = '0';
  } else {
    while (tmp) {
      buf[i--] = (char)('0' + (tmp % 10));
      tmp /= 10;
    }
  }
  int start = i + 1;
  int digits = 31 - i - 1; /* exclude final '\0' */
  /* put into global buffer (single buffered calls) */
  ft_buf_putstr(&buf[start], digits);
  len += digits;

  /* right padding */
  if (flags.left_align)
    len += ft_printf_pad(' ', pad_spaces);

  return (len);
}

static inline int ft_printf_putunbr(unsigned int n, t_flags flags) {
  int len = 0;
  int num_len = ft_printf_unumlen(n);

  if (!flags.left_align && !flags.zero_padding)
    len += ft_printf_pad(' ',
                         flags.width - ft_printf_max(flags.precision, num_len));
  if (!flags.left_align && flags.zero_padding && !flags.precision_specified)
    len += ft_printf_pad('0', flags.width - num_len);
  len += ft_printf_pad('0', flags.precision - num_len);

  char buffer[11];
  int i = 10;
  buffer[i--] = '\0';
  if (n == 0)
    buffer[i--] = '0';
  while (n) {
    buffer[i--] = '0' + (n % 10);
    n /= 10;
  }
  int start = i + 1;
  int to_write = 10 - start;
  ft_buf_putstr(&buffer[start], to_write); // modified
  if (flags.left_align)
    len += ft_printf_pad(' ',
                         flags.width - ft_printf_max(flags.precision, num_len));
  return (len);
}

static inline int ft_printf_puthex(unsigned long n, t_flags flags,
                                   int uppercase) {
  char *base = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
  char buffer[17];
  int i = 16;
  int len = 0;
  buffer[i--] = '\0';
  if (n == 0)
    buffer[i--] = '0';
  while (n) {
    buffer[i--] = base[n % 16];
    n /= 16;
  }
  int start = i + 1;
  int hex_len = 16 - start;
  if (!flags.left_align && !flags.zero_padding)
    len += ft_printf_pad(' ', flags.width - hex_len);
  if (!flags.left_align && flags.zero_padding)
    len += ft_printf_pad('0', flags.width - hex_len);
  ft_buf_putstr(&buffer[start], hex_len); // modified
  if (flags.left_align)
    len += ft_printf_pad(' ', flags.width - hex_len);
  return (len);
}

static inline int ft_printf_putptr(void *ptr, t_flags flags) {
  unsigned long addr = (unsigned long)ptr;
  int len = 0;
  char buffer[19];
  int i = 18;
  buffer[i--] = '\0';
  if (addr == 0)
    buffer[i--] = '0';
  while (addr) {
    buffer[i--] = "0123456789abcdef"[addr % 16];
    addr /= 16;
  }
  buffer[i--] = 'x';
  buffer[i--] = '0';
  int start = i + 1;
  int str_len = 18 - start;
  if (!flags.left_align)
    len += ft_printf_pad(' ', flags.width - str_len);
  ft_buf_putstr(&buffer[start], str_len); // modified
  if (flags.left_align)
    len += ft_printf_pad(' ', flags.width - str_len);
  return (len);
}

static inline long long ft_printf_power10(int n) {
  long long res = 1;
  while (n--)
    res *= 10;
  return (res);
}

static long long ft_printf_round_fraction(double x) {
  long long base = (long long)x;
  double diff = x - (double)base;
  if (diff >= 0.5)
    return (base + 1);
  return (base);
}

// this is a better implementation
static inline int ft_printf_putfloat(double n, t_flags flags) {
  int count = 0;
  char buf[128];
  int len = 0;

  // Treats NaN/Inf
  if (ft_isnan(n)) {
    return (ft_write(ft_current_fd, "nan", 3));
  }
  if (ft_isinf(n)) {
    return ((n > 0) ? ft_write(ft_current_fd, "inf", 3)
                    : ft_write(ft_current_fd, "-inf", 4));
  }

  // Signal
  int neg = 0;
  if (n < 0) {
    neg = 1;
    n = -n;
  }

  // Standard accuracy
  int precision = flags.precision_specified ? flags.precision : 6;

  // Parte inteira
  long long int_part = (long long)n;
  double frac = n - (double)int_part;

  // Converts integer part to string
  {
    char tmp[64];
    int i = 0;
    if (int_part == 0)
      tmp[i++] = '0';
    else {
      while (int_part > 0) {
        tmp[i++] = (int_part % 10) + '0';
        int_part /= 10;
      }
    }
    while (i--)
      buf[len++] = tmp[i];
  }

  // Fractional part
  if (precision > 0) {
    buf[len++] = '.';

    long long scale = ft_printf_power10(precision);
    long long frac_part = ft_printf_round_fraction(frac * (double)scale);

    // Corrects if rounded to 1,000...
    if (frac_part >= scale) {
      frac_part = 0;
      // increments integer part (already printed in buf)
      int i = len - 2;
      while (i >= 0 && buf[i] == '9') {
        if (buf[i] != '.')
          buf[i] = '0';
        i--;
      }
      if (i >= 0 && buf[i] != '.') {
        buf[i]++;
      } else {
        for (int j = len; j > 0; j--)
          buf[j] = buf[j - 1];
        buf[0] = '1';
        len++;
      }
    }

    // Write frac_part with leading zeros
    char tmp[64];
    int i = 0;
    if (frac_part == 0) {
      for (int j = 0; j < precision; j++)
        buf[len++] = '0';
    } else {
      while (i < precision) {
        tmp[i++] = (frac_part % 10) + '0';
        frac_part /= 10;
      }
      while (i--)
        buf[len++] = tmp[i];
    }
  }

  buf[len] = '\0';

  // Width + padding
  int total_len = len + (neg ? 1 : 0);
  int pad = flags.width > total_len ? flags.width - total_len : 0;

  if (!flags.left_align && !flags.zero_padding)
    count += ft_printf_putnchar(' ', pad);

  if (neg)
    count += ft_printf_putchar('-');

  if (!flags.left_align && flags.zero_padding)
    count += ft_printf_putnchar('0', pad);

  for (int i = 0; i < len; i++) {
    ft_buf_putchar(buf[i]); // modified
    count++;                // modified
  }

  if (flags.left_align)
    count += ft_printf_putnchar(' ', pad);

  return (count);
}

static inline int ft_printf_putpercent(t_flags flags) {
  int len = 0;
  if (!flags.left_align && !flags.zero_padding)
    len += ft_printf_pad(' ', flags.width - 1);
  if (!flags.left_align && flags.zero_padding)
    len += ft_printf_pad('0', flags.width - 1);
  ft_buf_putchar('%'); // modified
  len++;               // modified
  if (flags.left_align)
    len += ft_printf_pad(' ', flags.width - 1);
  return (len);
}

static int ft_printf_parse_number(const char **format) {
  int num = 0;
  while (**format >= '0' && **format <= '9') {
    num = num * 10 + (**format - '0');
    (*format)++;
  }
  return (num);
}

static inline t_flags ft_printf_parse_flags(const char **format) {
  t_flags flags = {0, 0, false, false, false, 0};

  while (**format == '-' || **format == '0') {
    if (**format == '-')
      flags.left_align = true;
    else if (**format == '0')
      flags.zero_padding = true;
    (*format)++;
  }

  if (**format >= '0' && **format <= '9')
    flags.width = ft_printf_parse_number(format);

  if (**format == '.') {
    (*format)++;
    flags.precision_specified = true;
    flags.precision = ft_printf_parse_number(format);
  }

  if (**format == 'l') {
    flags.length = 1;
    (*format)++;
    if (**format == 'l') {
      flags.length = 2;
      (*format)++;
    }
  }

  if (flags.left_align)
    flags.zero_padding = false;

  return (flags);
}

/*dispatch table implementation (drop-in) */

typedef int (*t_handler_fn)(va_list *args, t_flags flags);

/* Forward declarations of handlers */
static int h_char(va_list *args, t_flags flags);
static int h_string(va_list *args, t_flags flags);
static int h_signed(va_list *args, t_flags flags);
static int h_unsigned(va_list *args, t_flags flags);
static int h_hex(va_list *args, t_flags flags, int uppercase);
static int h_ptr(va_list *args, t_flags flags);
static int h_float(va_list *args, t_flags flags);
static int h_percent(va_list *args, t_flags flags);

/* Wrapper for hex uppercase/lower that matches t_handler_fn signature */
static int h_hex_lower(va_list *args, t_flags flags) {
  return (h_hex(args, flags, 0));
}
static int h_hex_upper(va_list *args, t_flags flags) {
  return (h_hex(args, flags, 1));
}

/* Dispatch table and init flag */
static t_handler_fn g_dispatch_table[256] = {0};
static int g_dispatch_inited = 0;

/* Lazy init - fill entries*/
static void ft_dispatch_init(void) {
  if (g_dispatch_inited)
    return;
  g_dispatch_inited = 1;

  g_dispatch_table[(unsigned char)'c'] = h_char;
  g_dispatch_table[(unsigned char)'s'] = h_string;
  g_dispatch_table[(unsigned char)'d'] = h_signed;
  g_dispatch_table[(unsigned char)'i'] = h_signed;
  g_dispatch_table[(unsigned char)'u'] = h_unsigned;
  g_dispatch_table[(unsigned char)'x'] = h_hex_lower;
  g_dispatch_table[(unsigned char)'X'] = h_hex_upper;
  g_dispatch_table[(unsigned char)'p'] = h_ptr;
  g_dispatch_table[(unsigned char)'f'] = h_float;
  g_dispatch_table[(unsigned char)'g'] = h_float;
  g_dispatch_table[(unsigned char)'%'] = h_percent;
}

/* Central dispatch function using the table.
   Note: takes va_list by value (as you have), but we pass its address to
   handlers. */
static inline int ft_printf_dispatch(char spec, va_list *args, t_flags flags) {
  ft_dispatch_init();
  t_handler_fn h = g_dispatch_table[(unsigned char)spec];
  if (!h) {
    /* unknown specifier: do nothing (or return 0). Avoid recursive printing
     * here. */
    return 0;
  }
  return (h(args, flags));
}

/* ----------------- Handlers ----------------- */

static int h_char(va_list *args, t_flags flags) {
  int c = va_arg(*args, int);
  return (ft_printf_putchar_base((char)c, flags.width, flags.left_align));
}

static int h_string(va_list *args, t_flags flags) {
  char *s = va_arg(*args, char *);
  return (ft_printf_putstr(s, flags));
}

static int h_signed(va_list *args, t_flags flags) {
  if (flags.length == 2) {
    long long v = va_arg(*args, long long);
    return (ft_printf_putnbr(v, flags));
  } else if (flags.length == 1) {
    long v = va_arg(*args, long);
    return (ft_printf_putnbr((long long)v, flags));
  } else {
    int v = va_arg(*args, int);
    return (ft_printf_putnbr((long long)v, flags));
  }
}

static int h_unsigned(va_list *args, t_flags flags) {
  if (flags.length == 2) {
    unsigned long long v = va_arg(*args, unsigned long long);
    return (ft_printf_putunbr((unsigned int)v, flags));
  } else if (flags.length == 1) {
    unsigned long v = va_arg(*args, unsigned long);
    return (ft_printf_putunbr((unsigned int)v, flags));
  } else {
    unsigned int v = va_arg(*args, unsigned int);
    return (ft_printf_putunbr(v, flags));
  }
}

static int h_hex(va_list *args, t_flags flags, int uppercase) {
  if (flags.length == 2) {
    unsigned long long v = va_arg(*args, unsigned long long);
    return (ft_printf_puthex((unsigned long)v, flags, uppercase));
  } else if (flags.length == 1) {
    unsigned long v = va_arg(*args, unsigned long);
    return (ft_printf_puthex(v, flags, uppercase));
  } else {
    unsigned int v = va_arg(*args, unsigned int);
    return (ft_printf_puthex(v, flags, uppercase));
  }
}

static int h_ptr(va_list *args, t_flags flags) {
  void *p = va_arg(*args, void *);
  return (ft_printf_putptr(p, flags));
}

static int h_float(va_list *args, t_flags flags) {
  double d = va_arg(*args, double);
  return (ft_printf_putfloat(d, flags));
}

static int h_percent(va_list *args, t_flags flags) {
  (void)args; /* no va_arg for % */
  return (ft_printf_putpercent(flags));
}

/* ft_printf: send to ff instead of hardcoding 1 */
int ft_vdprintf(int fd, const char *format, va_list *args) {
  int total = 0;
  const char *p = format;

  int prev_fd = ft_current_fd;
  ft_current_fd = fd;
  ft_buf_set_fd(fd);

  while (*p) {
    if (*p == '%') {
      p++;
      t_flags flags = ft_printf_parse_flags(&p);
      /* p now points to the specifier (e.g. 'd', 's', etc.) */
      total += ft_printf_dispatch(*p++, args, flags);
    } else {
      ft_buf_putchar(*p++);
      total++;
    }
  }

  ft_buf_flush();
  ft_current_fd = prev_fd;
  return (total);
}

/* convenient wrappers */
int ft_dprintf(int fd, const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  int r = ft_vdprintf(fd, format, &ap);
  va_end(ap);
  return (r);
}

int ft_fprintf(T_FT_FILE *stream, const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  int fd = ft_fileno(stream);
  int r = ft_vdprintf(fd, format, &ap);
  va_end(ap);
  return (r);
}

/* maintains compatibility with ft_printf()*/
int ft_printf(const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  int r = ft_vdprintf(1, format, &ap);
  va_end(ap);
  return (r);
}
