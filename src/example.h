/**
   Title: example
   Author: Jonas Jermiin Ravn Moll (jonas@jermiin.dk)
   Date: 04.05.2005

   Original version by Ben Tindale <ben@bluesat.unsw.edu.au>, published in "Linux Gazette", 2000, vol. 55.
   Only minor changes applied to the actual grep module - but added exception handling and done some restyling for better readability
 
   ----------------------------------------------------------------------------
   "THE BEER-WARE LICENSE" (Revision 42):
   <jonas@jermiin.dk> wrote this file. As long as you retain this notice you
   may do whatever you want with this stuff. If we meet some day, and you think
   this stuff is worth it, you can buy me a beer in return.
   ----------------------------------------------------------------------------
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
