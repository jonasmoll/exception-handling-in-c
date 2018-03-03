/**
   Title: cexcept.h
   Author: Jonas Jermiin Ravn Moll (jonas@jermiin.dk)
   Date: 04.05.2005

   Inspired by original library built by Adam M. Costello <amc@cs.berkeley.edu>
   Only minor changes applied to the actual grep module - but added exception handling and done some restyling for better readability

   Aim: Implementation of exception handling in c

   ----------------------------------------------------------------------------
   "THE BEER-WARE LICENSE" (Revision 42):
   <jonas@jermiin.dk> wrote this file. As long as you retain this notice you
   may do whatever you want with this stuff. If we meet some day, and you think
   this stuff is worth it, you can buy me a beer in return.
   ----------------------------------------------------------------------------
*/

#ifndef CEXCEPT_H
#define CEXCEPT_H
#include <setjmp.h>
//#include <string.h>

#define Handler_id the_exception_context->handler_id

enum {Except_entered = 0, Except_thrown,  Except_handled, Except_uncaught, Except_finalized};

#define define_exception_type(etype) \
	struct exception_context { \
		jmp_buf *penv; \
		const char *reason; \
		const char *handler_id; \
		volatile struct { etype etmp; } v; \
	}

#define init_exception_context(ec) ((void)((ec)->penv = 0))

#define Try \
	{ \
		jmp_buf *exception__prev, exception__env; \
		volatile int except_flag; \
		exception__prev = the_exception_context->penv; \
		the_exception_context->penv = &exception__env; \
		except_flag = setjmp(exception__env); \
		if (except_flag == Except_entered) { \
	
#define exception__catch(e) \
			if(except_flag == Except_entered) \
				the_exception_context->penv = exception__prev; \
		} \
		else if(except_flag == Except_thrown) \
			if(strcmp(e,the_exception_context->handler_id) == 0) { \
				except_flag = Except_handled;

#define exception__catchall \
				if(except_flag == Except_entered) \
					the_exception_context->penv = exception__prev; \
			} \
			else if(except_flag == Except_thrown) { \
					except_flag = Except_handled;				

#define exception__finally \
			} \
			else if(!(except_flag == Except_handled)) { \
				except_flag = Except_finalized; \
				the_exception_context->penv = exception__prev; \
				fprintf(stderr, "Uncaught exception:"); \
				if (the_exception_context->reason) \
					fprintf(stderr, "' %s'", the_exception_context->reason); \
				else \
					fprintf(stderr, " at %p", the_exception_context); \
				if (__FILE__ && __LINE__ > 0) \
					fprintf(stderr, " raised in %s, line %d (Compiled on the %s at %s)\n" \
							, __FILE__, __LINE__, __DATE__, __TIME__); \
				fprintf(stderr, "Terminating runtime environment...\n"); \
				exit(EXIT_FAILURE); \
			} \
	}

#define Catch(e) exception__catch(e)
#define Catch_all() exception__catchall
#define Finally exception__finally
	
#define Throw \
	except_flag = Except_thrown; \
	for (;; longjmp(*the_exception_context->penv, 1)) \
		the_exception_context->v.etmp =

#endif /* CEXCEPT_H */
