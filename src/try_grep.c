/**
   Title: trygrep
   Author: Jonas Jermiin Ravn Moll (jonas@jermiin.dk)

   Original version by Ben Tindale <ben@bluesat.unsw.edu.au>, published in "Linux Gazette", 2000, vol. 55.
   Only minor changes applied to the actual grep module - but added exception handling and done some restyling for better readability

   Description:
   A simple example implementation of the grep
   utility, using regular expressions from the GNU C library.
   See the GNU documentation for information about
   regular expressions: http://www.gnu.org
   This version uses the "cexcept.h" for allowing simple
   Exception handling instead of condition checking.

   Usage: trygrep -f <filename> -p <pattern>
 
   ----------------------------------------------------------------------------
   "THE BEER-WARE LICENSE" (Revision 42):
   <jonas@jermiin.dk> wrote this file. As long as you retain this notice you
   may do whatever you want with this stuff. If we meet some day, and you think
   this stuff is worth it, you can buy me a beer in return.
   ----------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <regex.h> 		/* Provides regular expression matching */
#include <strings.h> 	/* String utillity functions */
#include <errno.h> 		/* Handle errors */
#include "cexcept_ext.h"

/*! The maximum length of a single line */
#define MAXLENGHT 1024

enum exception_flavor { success, failure };

struct exception {
  enum exception_flavor flavor;
  const char *msg;
  union {
    int err;
    long err_long;
    char err_msg[8];
  } err_code;
};

define_exception_type(struct exception);

/* Specify globally exception_context */
extern struct exception_context the_exception_context[1];

/*! Matches each line in a given file with the specified string pattern
  \param regex_t Pointer to a pre-"compiled" regex_t (Regular Expression) structure.
  \param FH Filehandler for the input given file.
  \return 0 if the function call succeeded, otherwise a non-zero value indicating error
  \sa THE GNU implementation of regex.h
*/
int  match_patterns(regex_t *r, FILE *FH) {
  
  char *cp; 					/* Char pointer */
  char line[MAXLENGHT];			/* Initialize char array, assuming a 'line' is less than 1023 bytes long */
  char strip[MAXLENGHT];
  int line_num = 1;				/* Count the line numbers for nice output */
  size_t num_sub = r->re_nsub+1;/* How many matches are there in a line? */
  regmatch_t *result;			/* Pointer to 'regmatch_t' type structure - holding reference for start and end of match */
  int start = 0; 				/* The offset from the beginning of the line */

  if( (result = (regmatch_t *) malloc(sizeof(regmatch_t) * num_sub)) == 0 ) {
	perror("Could not allocate enough memory!");
    exit(EXIT_FAILURE);
  }

  /* Signature for the regex call
   * int regexec(const regex_t *, const char *, size_t, regmatch_t [], int);
   */
  while( (cp=fgets(line, MAXLENGHT-1, FH) ) != NULL) {			/* Read next line from file 'FH' until EOF. Return 'line'- as cp */
	while(regexec(r, line+start, num_sub, result, 0) == 0) { 	/* Found a match */
	  printf("Line %d: %s", line_num, line);
	  start +=result->rm_eo;									/* Update the offset */
	  printf("cp=%d\n",strip);
	}
	line_num++;
  }
  return EXIT_SUCCESS;
}

/*! Allocates the memory needed, creates a filehandler (FH) for the given file to be examined and "compiles" the search pattern into a Regular Expression used by match_pattern()
  \param regex_t Pointer to a regex_t (Regular Expression) structure.
  \param *p Reference to the search pattern
  \param *f Reference to the given file
  \return 0 if the function call succeeded, otherwise a non-zero value indicating error
  \sa THE GNU implementation of regex.h
*/
int do_regex(regex_t *r, char *p, char *f) {
  struct exception e;
  int err_num = 0; 		/* For regerror() */
  FILE *FH = NULL; 		/* File handle */

  /* Compile the expression with a zero flag and place the results in the regex_t structure.
	 If something goes wrong, make an error message and print it to stdout. */
  if( (err_num = regcomp(r, p, 0)) != 0 ) {
      char *buffer;
      
	  buffer = malloc( regerror(err_num, r, NULL, 0) );	/* Allocate memory required by the errbuf to contain the null-terminated error message string. And store the size in 'buffer' */
      regerror(err_num, r, buffer, sizeof(buffer));		/* Transform error codes into human-readable messages, and place in 'buffer' */
      fprintf(stderr, "%s\n", buffer); 					/* Print the error message */
      free(buffer);										/* Free up the space allocated for the message buffer */
      regfree(r);										/* Free any dynamically-allocated storage associated with the compiled expression */
      return EXIT_FAILURE;
  }
  
  if( (FH = fopen(f, "r")) == NULL ) {					/* Open the file to scan */
	e.flavor = failure;
    e.msg = "Could not open the specified file!";
    e.err_code.err = 99;
	Throw e;
  }
  
  match_patterns(r, FH);				/* Pass the pattern and the file to be scanned */
  regfree(r);							/* Free the regular expression data structure */
  free(r);
  fclose(FH);
  return EXIT_SUCCESS;
}

/*! Print usage info */
void usage() {
  printf("\nUsage: mygrep -f <filename> -p <pattern>\nExample: mygrep -f foo.c -p \'Foo Bar\'\n");
}

/* Globally accessible storage for the exception context: */
struct exception_context the_exception_context[1];

int main(int argc, char **argv) {
  int c;
  char *filename = NULL;
  extern char *optarg;
  extern int optind, opterr, optopt;
  char *pattern = NULL;					/* String to hold "pattern" */
  regex_t *regex;						/* Pointer to regex_t structure */
  struct exception e;								/* String to hold exception msg */ 

  regex = (regex_t *) malloc(sizeof(regex_t));			/* Allocate memory for the regular expression, and save the casted result in var regex */
  memset(regex, 0, sizeof(regex_t));					/* ??? */

  if(argc == 1) {										/* If wrong number of arguments - print usage info */
	usage();
    return (EXIT_SUCCESS);
  }

	while( (c = getopt(argc, argv, "f:p:")) != EOF ) {	/* Get option letter from arguments */
	  switch(c) {
		case 'f':
		  filename = optarg;
		  break;
		case 'p':
		  pattern = optarg;
		  break;
		case '?':
		  fprintf (stderr, "Unknown option `-%c'.\n", optopt);
		  usage();
		  return (EXIT_FAILURE);
		default:
		  usage();
		  return (EXIT_SUCCESS);
	}
  }
  
  Try {
	do_regex(regex, pattern, filename);
  }
  Catch (e) {
    switch (e.flavor) {
	  //case success:
	  break;
        case success:
		  fprintf(stderr, "Error: %s\n(Exception info: %s, Error code: %d)\n", "Blah", e.msg, e.err_code.err);
		  usage();
		  break;
		  //case screwup: fprintf(stderr, "main caught screwup (info == %ld): %s\n", e.info.screwup, e.msg);
        default:
		  fprintf(stderr, "Caught unknown exception\n");
		  usage();
    }
  }
  exit(EXIT_SUCCESS);
}
