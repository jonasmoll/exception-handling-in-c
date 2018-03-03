class Java_test {
	public static void main(String [] args) { new test(1); }
}

class test {
	long before, after, delta;
	
	public test(int x) {
		if(x == 0) {
			before = System.currentTimeMillis();
			for(int i = 0; i < 1000; i++) {
				try {
					testFunction(i);
				}
				catch(Exception e){}
			}
		}
		else if(x == 1) {
			before = System.currentTimeMillis();
			for(int i = 0; i < 1000; i++) {
				try {
					testFunction2(i);
				}
				catch(Exception e){
					//System.out.println("Exception no. "+i+" caught.");
				}
			}
		}
		after = System.currentTimeMillis();
		delta = after - before;
		System.out.println("Time elapsed: " + delta + " ms.");
	}
	int testFunction(int a) {
		return a + a;
	}

	int testFunction2(int a) {
		throw new java.lang.ArithmeticException();
	}
}
