#include "../include/ft_maki.h"

int ft_printf_numlen(long long n)
{
    // 0 -> 1 (for '0'), negative -> count sign outside if you prefer
    int len = (n <= 0) ? 1 : 0;
    long long tmp = (n < 0) ? -n : n;
    while (tmp) {
        len++;
        tmp /= 10;
    }
    return (len);
}

int ft_printf_unumlen(unsigned long n)
{
    int len = 1;
    while (n >= 10) {
        len++;
        n /= 10;
    }
    return (len);
}

int ft_printf_max(int a, int b)
{
    return ((a > b) ? a : b);
}

int ft_printf_putchar(char c)
{
    return (ft_write(1, &c, 1));
}

int ft_printf_putnchar(char c, int n)
{
    int count = 0;
    while (n-- > 0)
        count += ft_printf_putchar(c);
    return (count);
}

int ft_printf_putchar_base(char c, int width, int left)
{
    int printed = 0;

    if (!left)
        for (int i = 0; i < width - 1; i++)
            printed += ft_write(1, " ", 1);

    printed += ft_write(1, &c, 1);

    if (left)
        for (int i = 0; i < width - 1; i++)
            printed += ft_write(1, " ", 1);

    return (printed);
}

int ft_printf_putstr(char *s, t_flags flags)
{
    int len = 0;
    if (!s)
        s = "(null)";
    int str_len = 0;
    while (s[str_len] && (!flags.precision_specified || str_len < flags.precision))
        str_len++;
    if (!flags.left_align)
        len += ft_printf_pad(' ', flags.width - str_len);
    len += ft_write(1, s, str_len);
    if (flags.left_align)
        len += ft_printf_pad(' ', flags.width - str_len);
    return (len);
}

int ft_printf_putnbr(long long n, t_flags flags)
{
    int len = 0;
    long long tmp = (n < 0) ? -n : n;
    int num_len = ft_printf_numlen(tmp);
    int total_width = ft_printf_max(num_len, flags.precision);
    if (n < 0)
        total_width++;
    if (!flags.left_align && !flags.zero_padding)
        len += ft_printf_pad(' ', flags.width - total_width);
    if (n < 0)
        len += ft_write(1, "-", 1);
    if (!flags.left_align && flags.zero_padding)
        len += ft_printf_pad('0', flags.width - total_width);
    len += ft_printf_pad('0', flags.precision - num_len);
    char buffer[21];
    int i = 20;
    buffer[i--] = '\0';
    if (tmp == 0)
        buffer[i--] = '0';
    while (tmp) {
        buffer[i--] = '0' + (tmp % 10);
        tmp /= 10;
    }
    int start = i + 1;
    int to_write = 20 - start; // = buffer_size - start - 1
    len += ft_write(1, &buffer[start], to_write);
    if (flags.left_align)
        len += ft_printf_pad(' ', flags.width - total_width);
    return (len);
}


int ft_printf_putunbr(unsigned long n, t_flags flags)
{
    int len = 0;
    int num_len = ft_printf_unumlen(n);

    if (!flags.left_align && !flags.zero_padding)
        len += ft_printf_pad(' ', flags.width - ft_printf_max(flags.precision, num_len));
    if (!flags.left_align && flags.zero_padding && !flags.precision_specified)
        len += ft_printf_pad('0', flags.width - num_len);
    len += ft_printf_pad('0', flags.precision - num_len);

    char buffer[21];
    int i = 20;
    buffer[i--] = '\0';
    if (n == 0)
        buffer[i--] = '0';
    while (n)
    {
        buffer[i--] = '0' + (n % 10);
        n /= 10;
    }
    int start = i + 1;
    int to_write = 20 - start;
    len += ft_write(1, &buffer[start], to_write);
    if (flags.left_align)
        len += ft_printf_pad(' ', flags.width - ft_printf_max(flags.precision, num_len));
    return (len);
}

int ft_printf_puthex(unsigned long n, t_flags flags, int uppercase)
{
    const char *base = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    char buffer[17];
    int i = 16;
    int len = 0;
    buffer[i--] = '\0';
    if (n == 0)
        buffer[i--] = '0';
    while (n)
    {
        buffer[i--] = base[n % 16];
        n /= 16;
    }
    int start = i + 1;
    int hex_len = 16 - start; //number of real digits
    if (!flags.left_align && !flags.zero_padding)
        len += ft_printf_pad(' ', flags.width - hex_len);
    if (!flags.left_align && flags.zero_padding)
        len += ft_printf_pad('0', flags.width - hex_len);
    len += ft_write(1, &buffer[start], hex_len);
    if (flags.left_align)
        len += ft_printf_pad(' ', flags.width - hex_len);
    return (len);
}

int ft_printf_putptr(void *ptr, t_flags flags)
{
    unsigned long addr = (unsigned long)ptr;
    int len = 0;
    char buffer[19];
    int i = 18;
    buffer[i--] = '\0';
    if (addr == 0)
        buffer[i--] = '0';
    while (addr)
    {
        buffer[i--] = "0123456789abcdef"[addr % 16];
        addr /= 16;
    }
    buffer[i--] = 'x';
    buffer[i--] = '0';
    int start = i + 1;
    int str_len = 18 - start; //18 = total size - 1 (reserved '\0')
    if (!flags.left_align)
        len += ft_printf_pad(' ', flags.width - str_len);
    len += ft_write(1, &buffer[start], str_len);
    if (flags.left_align)
        len += ft_printf_pad(' ', flags.width - str_len);
    return (len);
}

long long ft_printf_power10(int n)
{
    long long res = 1;
    while (n--) res *= 10;
    return (res);
}

static long long ft_printf_round_fraction(double x)
{
    long long base = (long long)x;
    double diff = x - (double)base;
    if (diff >= 0.5)
        return (base + 1);
    return (base);
}

//this is a better implementation
int ft_printf_putfloat(double n, t_flags flags)
{
    int count = 0;
    char buf[128];
    int len = 0;

    // Treats NaN/Inf
    if (ft_isnan(n))
    {
        return (ft_write(1, "nan", 3));
    } 
    if (ft_isinf(n))
    {
        return ((n > 0) ? 
        ft_write(1, "inf", 3) : ft_write(1, "-inf", 4));
    }    

    // Signal
    int neg = 0;
    if (n < 0)
    {
        neg = 1;
        n = -n;
    }

    // Standard accuracy
    int precision = flags.precision_specified ? flags.precision : 6;

    // Parte inteira
    long long int_part = (long long) n;
    double frac = n - (double) int_part;

    // Converts integer part to string
    {
        char tmp[64];
        int i = 0;
        if (int_part == 0)
            tmp[i++] = '0';
        else
        {
            while (int_part > 0)
            {
                tmp[i++] = (int_part % 10) + '0';
                int_part /= 10;
            }
        }
        while (i--) buf[len++] = tmp[i];
    }

    // Fractional part
    if (precision > 0)
    {
        buf[len++] = '.';

        long long scale = ft_printf_power10(precision);
        long long frac_part = ft_printf_round_fraction(frac * (double)scale);

        // Corrects if rounded to 1,000...
        if (frac_part >= scale) {
            frac_part = 0;
            // increments integer part (already printed in buf)
            int i = len - 2;
            while (i >= 0 && buf[i] == '9')
            {
                if (buf[i] != '.') buf[i] = '0';
                i--;
            }
            if (i >= 0 && buf[i] != '.')
            {
                buf[i]++;
            } else
            {
                for (int j = len; j > 0; j--) buf[j] = buf[j-1];
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
        } 
        else
        {
            while (i < precision)
            {
                tmp[i++] = (frac_part % 10) + '0';
                frac_part /= 10;
            }
            while (i--) buf[len++] = tmp[i];
        }
    }

    buf[len] = '\0';

    // Largura + padding
    int total_len = len + (neg ? 1 : 0);
    int pad = flags.width > total_len ? flags.width - total_len : 0;

    if (!flags.left_align && !flags.zero_padding)
        count += ft_printf_putnchar(' ', pad);

    if (neg)
        count += ft_printf_putchar('-');

    if (!flags.left_align && flags.zero_padding)
        count += ft_printf_putnchar('0', pad);

    for (int i = 0; i < len; i++)
        count += ft_printf_putchar(buf[i]);

    if (flags.left_align)
        count += ft_printf_putnchar(' ', pad);

    return (count);
}

int ft_printf_putpercent(t_flags flags)
{
    int len = 0;
    if (!flags.left_align && !flags.zero_padding)
        len += ft_printf_pad(' ', flags.width - 1);
    if (!flags.left_align && flags.zero_padding)
        len += ft_printf_pad('0', flags.width - 1);
    len += ft_write(1, "%", 1);
    if (flags.left_align)
        len += ft_printf_pad(' ', flags.width - 1);
    return (len);
}

//write using buffer
int ft_printf_pad(char c, int count)
{
    if (count <= 0) return 0;
    char buf[64];
    ft_memset(buf, c, sizeof(buf));
    int len = 0;
    while (count > 0)
    {
        int chunk = (count > 64) ? 64 : count;
        len += ft_write(1, buf, chunk);
        count -= chunk;
    }
    return (len);
}

static int ft_printf_parse_number(const char **format)
{
    int num = 0;
    while (**format >= '0' && **format <= '9')
    {
        num = num * 10 + (**format - '0');
        (*format)++;
    }
    return (num);
}

t_flags ft_printf_parse_flags(const char **format)
{
    t_flags flags = {0, 0, false, false, false};

    while (**format == '-' || **format == '0')
    {
        if (**format == '-')
            flags.left_align = true;
        else if (**format == '0')
            flags.zero_padding = true;
        (*format)++;
    }

    if (**format >= '0' && **format <= '9')
        flags.width = ft_printf_parse_number(format);

    if (**format == '.')
    {
        (*format)++;
        flags.precision_specified = true;
        flags.precision = ft_printf_parse_number(format);
    }

    if (flags.left_align)
        flags.zero_padding = false;

    return (flags);
}

int ft_printf_dispatch(char spec, va_list args, t_flags flags)
{
    if (spec == 'c')
        return ft_printf_putchar_base((char)va_arg(args, int), flags.width, flags.left_align);
    else if (spec == 's')
        return ft_printf_putstr(va_arg(args, char *), flags);
    else if (spec == 'd' || spec == 'i')
	{
        // if you want long long, use va_arg(args, long long)
		return ft_printf_putnbr(va_arg(args, int), flags);
	}
    else if (spec == 'u')
	{
		// or unsigned long if changing the function
		return ft_printf_putunbr(va_arg(args, unsigned int), flags);
	}
    else if (spec == 'x')
        return ft_printf_puthex(va_arg(args, unsigned int), flags, 0);
    else if (spec == 'X')
        return ft_printf_puthex(va_arg(args, unsigned int), flags, 1);
    else if (spec == 'p')
        return ft_printf_putptr(va_arg(args, void *), flags);
    else if (spec == 'f' || spec == 'g')
        return ft_printf_putfloat(va_arg(args, double), flags);
    else if (spec == '%')
        return ft_printf_putpercent(flags);
    return (0);
}

int ft_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int total = 0;
    while (*format) {
        if (*format == '%') {
            format++;
            t_flags flags = ft_printf_parse_flags(&format);
            total += ft_printf_dispatch(*format++, args, flags);
        } else {
            total += ft_write(1, format++, 1);
        }
    }
    va_end(args);
    return (total);
}  

