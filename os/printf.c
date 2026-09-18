#include "kernel.h"

static int vsnprintf(char *out, size_t n, const char *s, va_list vl)
{
	int in_format = 0;
	int is_long = 0;
	size_t pos = 0;
	for (; *s; s++) {
		if (in_format) {
			switch(*s) {
			case 'l': {
				is_long = 1;
				break;
			}
			case 'p': {
				is_long = 1;
				if (out && pos < n) {
					out[pos] = '0';
				}
				pos++;
				if (out && pos < n) {
					out[pos] = 'x';
				}
				pos++;
			}
			case 'x': {
				long num = is_long ? va_arg(vl, long) : va_arg(vl, int);
				int hexdigits = 2*(is_long ? sizeof(long) : sizeof(int))-1;
				for(int i = hexdigits; i >= 0; i--) {
					int d = (num >> (4*i)) & 0xF;
					if (out && pos < n) {
						out[pos] = (d < 10 ? '0'+d : 'a'+d-10);
					}
					pos++;
				}
				is_long = 0;
				in_format = 0;
				break;
			}
			case 'd': {
				long num = is_long ? va_arg(vl, long) : va_arg(vl, int);
				if (num < 0) {
					num = -num;
					if (out && pos < n) {
						out[pos] = '-';
					}
					pos++;
				}
				long digits = 1;
				for (long nn = num; nn /= 10; digits++);
				for (int i = digits-1; i >= 0; i--) {
					if (out && pos + i < n) {
						out[pos + i] = '0' + (num % 10);
					}
					num /= 10;
				}
				pos += digits;
				is_long = 0;
				in_format = 0;
				break;
			}
			case 's': {
				const char *s2 = va_arg(vl, const char *);
				while (*s2) {
					if (out && pos < n) {
						out[pos] = *s2;
					}
					pos++;
					s2++;
				}
				is_long = 0;
				in_format = 0;
				break;
			}
			case 'c': {
				if (out && pos < n) {
					out[pos] = (char)va_arg(vl,int);
				}
				pos++;
				is_long = 0;
				in_format = 0;
				break;
			}
			default:
				break;
			}
		} else if (*s == '%') {
			in_format = 1;
		} else {
			if (out && pos < n) {
				out[pos] = *s;
			}
			pos++;
		}
    	}
	if (out && pos < n) {
		out[pos] = 0;
	} else if (out && n) {
		out[n-1] = 0;
	}
	return pos;
}

static char out_buf[1000];

static int vprintf(const char *s, va_list vl)
{
	int res = vsnprintf(NULL, -1, s, vl);
	if (res+1 >= sizeof(out_buf)) {
		uart_puts("Error: output string size overflow\n");
		while(1) {}
	}
	vsnprintf(out_buf, res + 1, s, vl);
	uart_puts(out_buf);
	return res;
}

int printf(const char *s, ...)
{
	int res = 0;
	va_list vl;
	va_start(vl, s);
	res = vprintf(s, vl);
	va_end(vl);
	return res;
}

void panic(const char *s)
{
	printf("panic: ");
	printf("%s", s);
	printf("\n");
	while (1) {};
}
