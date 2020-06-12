#include <stdio.h>
#include <stdarg.h>

#include "io.h"
#include "type.h"
#include "tree.h"
#include "list.h"
#include "hashtab.h"
#include "array.h"
#include "bigint.h"
#include "dynamic.h"
#include "string.h"

#define INDEX(ptr, init) (ptr-init)

static void _printf_io(char *fmt, va_list args);

extern void printf_io(char *fmt, ...) {
	va_list factor;
    va_start(factor, fmt);
	_printf_io(fmt, factor);
	va_end(factor);
}

extern void inputs_io(char *buffer, size_t size) {
	if (size == 0) {
		return;
	}
    char *ptr = buffer;
    while((*ptr = getchar()) != '\n' && INDEX(ptr, buffer) < (size-1)) {
        ++ptr;
    }
    *ptr = '\0';
}

static void _printf_io(char *fmt, va_list args) {
	_Bool flag = 0;
    while(*fmt) {
        switch(*fmt) {
        	case '%': 
        		flag = 1;
        		break;
        	default:
        		putchar(*fmt);
        }
        if (flag) {
        	++fmt;
        	switch(*fmt){
                case '%': putchar('%'); break;
        		case 'd': printf("%d", va_arg(args, int)); break;
        		case 'f': printf("%f", va_arg(args, double)); break;
        		case 's': printf("%s", va_arg(args, char*)); break;
        		case 'c': printf("%c", va_arg(args, int)); break;
        		case 'o': printf("%o", va_arg(args, int)); break;
        		case 'x': printf("%x", va_arg(args, int)); break;
        		case 'L': print_list(va_arg(args, List*)); break;
        		case 'T': print_tree(va_arg(args, Tree*)); break;
        		case 'H': print_hashtab(va_arg(args, HashTab*)); break;
        		case 'B': print_bigint(va_arg(args, BigInt*)); break;
        		case 'A': print_array(va_arg(args, Array*)); break;
        		case 'Z': print_stack(va_arg(args, Array*)); break;
                case 'S': print_string(va_arg(args, String*)); break;
                case 'D': print_dynamic(va_arg(args, Dynamic*)); break;
        	    default:  putchar(*fmt);
            }
        }
        flag = 0;
        ++fmt;
    }
}
