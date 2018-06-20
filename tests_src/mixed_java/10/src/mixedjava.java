class MixedJava {
	
	int i = 5;
	
	MixedJava(){
		
	}
	
	MixedJava(int val){
		this.i = val;
	}
	
	static {
		System.loadLibrary("MixedJava");
	}
	
	public native static MixedJava alloc_object();
	public native static MixedJava new_object();
	public native static SubMixedJava new_subclass_object();
	public native static int isinstanceof(MixedJava obj, String class_name);
	public native static int issameobject(MixedJava obj1, MixedJava obj2);
	
	public static void test_jni_alloc_object(){
		MixedJava obj = alloc_object();
		int i0 = obj.i;
		break_();
	}
	
	public static void test_jni_new_object(){
		MixedJava obj = new_object();
		int i0 = obj.i;
		break_();
	}
	
	public static void test_jni_new_subclass_object(){
		SubMixedJava obj = new_subclass_object();
		int i0 = obj.i;
		break_();
	}
	
	public static void test_jni_isinstanceof(){
		MixedJava obj = new MixedJava();
		SubMixedJava sub_obj = new SubMixedJava();
		int i0 = isinstanceof(sub_obj, "SubMixedJava");
		int i1 = isinstanceof(sub_obj, "MixedJava");		
		int i2 = isinstanceof(obj, "SubMixedJava");
		int i3 = isinstanceof(obj, "MixedJava");
		break_();
	}
	
	public static void test_jni_issameobject(){
		MixedJava obj1 = new MixedJava();
		MixedJava obj2 = new MixedJava();
		int i0 = issameobject(obj1, obj2);
		int i1 = issameobject(obj1, obj1);
	}
	
	//
	// MISC
	//
	
	public static void main(String[] args) throws Exception {
		test_jni_issameobject();
	}

	public static void break_(){
		/* Add an additional basic block to the function, so the results
		   of the last statements are not deleted, when returning.
		*/
	}
	
}

class SubMixedJava extends MixedJava {
	
	int j;
	
	SubMixedJava(){
		super();
	}
	
	SubMixedJava(int val){
		super(val+1);
	}
	
}