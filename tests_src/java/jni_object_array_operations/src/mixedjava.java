class MixedJava {
	
	int i;
		
	public native static void set_object_array_elem(MixedJava arr[], MixedJava obj, int idx);
	public native static MixedJava get_object_array_elem(MixedJava arr[], int idx);
	public native static MixedJava[] create_new_object_array(MixedJava obj);
	
	
	public static void test_jni_new_object_array(){
		MixedJava obj = new MixedJava();
		MixedJava arr[] = create_new_object_array(obj);
		obj.i = 10;
		int i0 = arr[3].i;
		break_();
	}
	
	public static void test_jni_access_object_array(){
		MixedJava arr[] = new MixedJava[5];
		MixedJava obj = new MixedJava();
		obj.i = 7;
		set_object_array_elem(arr, obj, 2);
		MixedJava obj2 = get_object_array_elem(arr, 2);
		int i0 = obj.i;
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
