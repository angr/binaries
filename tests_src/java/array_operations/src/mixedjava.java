class MixedJava {
	
	//
	// Index out of bounds
	//
	
	static void test_index_of_of_bound0() throws Exception {
		int len = System.in.read();
		int[] arr = new int[len];
		int x = arr.length;
		break_();
	}
	
	static void test_index_of_of_bound1() throws Exception {
		int len = System.in.read();
		int[] arr = new int[len];
		arr[100] = 5;
		int x = arr.length;
		break_();
	}
	
	static void test_index_of_of_bound2() throws Exception {
		int[] arr = new int[5];
		int idx = System.in.read();
		int a = arr[0];
		int b = arr[5];
		int c = arr[-1];
		int d = arr[idx];
		int e = arr[idx+1000];
		break_();
	}
	
	static void test_index_of_of_bound3() throws Exception {
		int[] arr = new int[10000];
		int idx = System.in.read();
		int a = arr[0];
		int b = arr[999];
		int c = arr[1000];
		int d = arr[idx+500];
		int e = arr[idx+1000];
		break_();
	}
	
	static void test_index_of_of_bound4() throws Exception {
		int idx = System.in.read();
		int[] arr = new int[10000+idx];
		int a = arr[0];
		int b = arr[999];
		int c = arr[1000];
		break_();
	}
	
	static void test_index_of_of_bound5() throws Exception {
		int idx = System.in.read();
		int[] arr = new int[idx+800];
		int a = arr[0];
		int b = arr[999];
		int c = arr[1000];
		break_();
	}
	
	//
	// Symbolic Indexes and Length
	//
	
	static void test_symbolic_array_length() throws Exception{
		int length = System.in.read();
		int[] arr = new int[length];
		arr['E'] = 5;
		if (arr[arr.length-1] == 5){
			System.out.write('W');
			System.out.flush();	
		} else {
			System.out.write('L');
			System.out.flush();	
		}	
	}
		
	static void test_symbolic_array_read() throws Exception {
		int[] arr = new int[256];
		// winning idxes are 65 and 67
		int winning_idx = 'A';
		arr[winning_idx] = 1;
		arr['C'] = 1;
		// game
		int idx = System.in.read();
		if (idx < 100) {
			int elem = arr[idx];
			if (elem == 1){
				System.out.write('W');
				System.out.flush();
				return;
			}
		} 
		System.out.write('L');
		System.out.flush();	
	}
	
	static void test_symbolic_array_write() throws Exception {
		int[] arr = new int[100];
		int idx = System.in.read();
		int val = System.in.read();
		arr[idx] = '5';
		if (arr['I'] == val) {
			System.out.write('W');
			System.out.flush();
		} else {
			System.out.write('L');
			System.out.flush();		
		}
	}
	
	//
	// Basic Array
	//
	
	static void test_basic_array_operations(){		
		int[] arr = {4, 3, 2, 1, 0};
		// 0 3 2 1 0
		arr[0] = arr[4];
		// 0 1 2 1 0
		int idx = arr[3];
		arr[idx] = 1;
		// 0 1 2 1 0
		arr[2] = arr[2];
		// 0 1 2 3 0
		arr[3] = arr[3] + 2;
		// 0 1 2 3 4
		arr[4] = 4;
		// access "unitialized" elements
		int[] arr2 = new int[2];
		arr2[0] = 2;
		int a, b, c, d, e, l, g, j;
		a = arr[0];
		b = arr[1];
		c = arr[2];
		d = arr[3];
		e = arr[4];
		l = arr.length;
		g = arr2[0];
		j = arr2[1];
		break_();
	}
	
    	/*
	 *  MISC
	 */
	
	static {
		System.loadLibrary("MixedJava");
	}
	
	public static void main(String[] args) throws Exception {
		
	}

	public static void break_(){
	/* Add an additional basic block to the function, so the results
	 * of the last statements are not deleted when returning.
	 */
	}
	
}