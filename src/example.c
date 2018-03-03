/**
   Title: example
   Author: Jonas Jermiin Ravn Moll (jonas@jermiin.dk)
   Date: 04.05.2005

   Inspired by original library built by Adam M. Costello <amc@cs.berkeley.edu>
   Only minor changes applied to the actual grep module - but added exception handling and done some restyling for better readability
 
   ----------------------------------------------------------------------------
   "THE BEER-WARE LICENSE" (Revision 42):
   <jonas@jermiin.dk> wrote this file. As long as you retain this notice you
   may do whatever you want with this stuff. If we meet some day, and you think
   this stuff is worth it, you can buy me a beer in return.
   ----------------------------------------------------------------------------
*/

#ifndef STRING_H
#include <string.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include "example.h"

void test_nested_try();
void test_catch_all();

int main(void) {
	/* Globally accessible storage for the exception context: */
	struct exception_context the_exception_context[1];
	struct exception context;
	int test = 2;
	
	Try {
		if(test == 0) {
			printf("Inside first stmt - test is: %d\n", test);
		}
		else if(test == 1) {
			Handler_id = "First Try";
			Throw context;
		}
		else if(test == 2) {
			context.err_msg[0] = "This is Thrown from the second Throw!";
			Handler_id = "1st Try - 2nd Catch";
			Throw context;
		}
		else if(test == 3) {
			Handler_id = "Missing Catch";
			Throw context;
		}
		else if(test == 4)
			test_nested_try(the_exception_context[1]);
		
		else if(test == 5)
			test_catch_all(the_exception_context[1]);
	}
	Catch("1st Try")
		printf("Caught exception - And handler is: %s\n", Handler_id);
	Catch("1st Try - 2nd Catch")
		printf("Caught exception - And message is: %s\n", context.err_msg[0]);	
	Finally
		printf("Inside Finally\n");
	return EXIT_SUCCESS;
}

void test_nested_try(struct exception_context *the_exp) {
	struct exception_context *the_exception_context = the_exp;
	struct exception exp;
	int test = 1;
	Try {
		if(test == 0) {
			printf("Inside nested Try..\n");
		}
		else if(test == 1) {
			Handler_id = "2nd Try";
			Throw exp;
		}
	}
	Catch("2nd Try") {
		printf("Caught exception in local Catch - And handler is: %s\n", Handler_id);
	}
	Finally{}
}

void test_catch_all(struct exception_context *the_exp) {
	struct exception_context *the_exception_context = the_exp;
	struct exception exp;
	int test = 1;
	Try {
		if(test == 0) {
			printf("Inside nested_catch_all...\n");
		}
		else if(test == 1) {
			Throw exp;
		}
		else if(test == 2) {
			Throw exp;
		}
	}
	Catch_all() {
		printf("Caught exception in Catch_all:\n");
	}
	Finally{}
}
