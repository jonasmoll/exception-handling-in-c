/**
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <jonas@jermiin.dk> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a beer in return
 * ----------------------------------------------------------------------------
 */

#include "cexcept_ext.h"
#define MAX_LENGTH 1024

struct exception {
		int err;
		long err_long;
		const char *err_msg[MAX_LENGTH];
};

define_exception_type(struct exception);
extern struct exception_context the_exception_context[1];
