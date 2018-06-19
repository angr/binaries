class MixedJava {

	static {
		System.loadLibrary("MixedJava");
	}
	
	public static native String concat_string(String str1, String str2);
	public static native int strlen(String str);
	
	public static void test_jni_string_operations(){
		String r0 = "mum";
		String r1 = concat_string("hi", r0);
		int i0 = strlen(r0);
		int i1 = strlen(r1);
		break_();
	}
	
	//
	// MISC
	//
	
	public static void main(String[] args) throws Exception {
		test_jni_string_operations();
	}

	public static void break_(){
		/* Add an additional basic block to the function, so the results
		   of the last statements are not deleted, when returning.
		*/
	}
	
}