class MixedJava {
	
	int f1;
	int f2 = 2;
	static int s1;
	static int s2 = 1;
	
	static {
		System.loadLibrary("MixedJava");
	}
	
	//
	// Instance Field Access
	//

	public native void set_field_f1(int val);
	public native int get_field_f1();
	public native int get_field_f2();
	
	public static void test_instance_field_access_0(){
		MixedJava obj = new MixedJava();
		int i0 = obj.f1; 				// 0
		obj.f1 = 10;
		int i1 = obj.f1; 				// 10
		obj.set_field_f1(5);
		int i2 = obj.f1; 				// 5
		int i3 = obj.get_field_f1();			// 5
		break_();
	}
	
	public static void test_instance_field_access_1(){
		SubMixedJava obj = new SubMixedJava();
		int a = obj.f1; 				// 0
		int b = obj.f2; 				// 1
		obj.f1 = 10;
		int c = obj.f1; 				// 10
		obj.set_field_f1(4);
		int d = obj.f1;					// 4
		int e = obj.get_field_f1();			// 4
		int f = obj.get_field_f2(); 			// 1
		break_();
	}

	// 
	// Static Field Access
	//
	
	public static void test_static_field_access_basic(){
		int i0 = s1;
		int i1 = s2;
		s1 = 0xA;
		s2 = 0xB;
		int i2 = s1;
		int i3 = s2;
		int i4 = SubMixedJava.s1;
		int i5 = SubMixedJava.s2;
		int i6 = SubMixedJava.s3;
		int i7 = SubMixedJava.s4;
		break_();
	}
	
	public native static int get_static_field();
	public native static void set_static_field(int val);
	public static void test_jni_static_field_access(){
		int i0 = get_static_field();
		set_static_field(5);
		int i1 = s2;
		break_();
	}
	
	public native static int get_static_field_access_subclass();
	public static void test_jni_static_field_access_subclass(){
		int i0 = get_static_field_access_subclass();
		int i1 = SubMixedJava.s1;
		int i2 = SubMixedJava.s3;
		int i3 = MixedJava.s2;
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

class SubMixedJava extends MixedJava {
	int f2 = 1;
	static int s1 = 7;
	static int s3;
	static int s4 = 9;
}
