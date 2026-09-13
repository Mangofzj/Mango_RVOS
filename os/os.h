#ifndef __OS_H__
#define __OS_H__

#include "types.h"
#include "platform.h"

#include <stddef.h>
#include <stdarg.h>

extern int uart_putc(char ch);
extern void uart_puts(char *s);

extern int  printf(const char* s, ...);
extern void panic(const char *s);

extern void *page_alloc(int npages);
extern void page_free(void *addr);

#endif /* __OS_H__ */
