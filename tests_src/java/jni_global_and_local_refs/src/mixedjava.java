class MixedJava {
	
	int i;
	
	public static native void create_global_ref(MixedJava obj);
	public static native void delete_global_ref();
	public static native int access_global_ref();

	public static void test_jni_global_refs(){
		MixedJava obj = new MixedJava();
		create_global_ref(obj);
		obj.i = 10;
		int i0 = access_global_ref();
		delete_global_ref();
		//int i1 = access_global_ref();
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
