/**
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <jonas@jermiin.dk> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a beer in return
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include "c_test.h"

struct timeval before, after;
int delta, i, x = 1;

int main(void) {
  // Globally accessible storage for the exception context:
  struct exception_context the_exception_context[1];
  struct exception context;
  if(x == 0) {
	gettimeofday(&before, NULL);
 	for(i = 0; i < 1000; i++) {
	  Try {
		test_function(i);
      }
      Catch_all() {
		//printf("Caught 'invalid argument' exception");
      }
	  Finally{}
	}
  } 
  if(x == 1) {
   gettimeofday(&before, NULL);
 	for(i = 0; i < 1000; i++) {
	  Try {
		test_function(i);
		Handler_id = "Invalid arg";
        Throw context;
	  }
      Catch("Invalid arg") {
		//printf("Caught 'invalid argument' exception");
      }
	  Finally{}
	}
  }
  gettimeofday(&after, NULL);
  delta = (after.tv_sec - before.tv_sec) * 1000000 + (after.tv_usec - before.tv_usec);
  printf("Benchmark for this run: %d ms.\n", delta);
  return EXIT_SUCCESS;         
}	

int test_function(int a) {
   return a + a;
}
