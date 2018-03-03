#include <iostream>
#include <string>
#include <sys/time.h>
#include <stdexcept>
using namespace std; 

int test_function(int a);
int test_function2(int a);

int main() {
  int delta;
  struct timeval before, after;
  int x = 1;

  if(x == 0) {
	  gettimeofday(&before, NULL);
  	  for(int i = 0; i < 1000; i++) {
		try {
			test_function(i);
		}
		catch(...){}
	  }
	}
	else if(x == 1) {
	  gettimeofday(&before, NULL);
	  for(int i = 0; i < 1000; i++) {
		try {
		  test_function2(i);
		}
		catch(invalid_argument& err){
		  //cerr << err.what() << endl;
		}
	  }
	}
	gettimeofday(&after, NULL);
	delta = (after.tv_sec - before.tv_sec) * 1000000 + (after.tv_usec - before.tv_usec);
	cout << "Time elapsed: " << delta << "ms." << endl;
  }

int test_function(int a) {
  return a+a;
}

int test_function2(int a) {
	throw invalid_argument("Invalid arg");
}
