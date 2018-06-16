class MixedJava {
	
	int i;
	
	//
	// JNI Instance Method Calls
	//
	
	// test group 1
	
	public native int native_callback();
	public static native void native_callback_2(MixedJava obj1, MixedJava ob2);
		
	public int callback(int val){
		this.i = val;
		return this.i;
	}
	
	public static void test_jni_instance_method_calls_basic(){
		MixedJava obj = new MixedJava();
		int a = obj.native_callback(); // 7
		int b = obj.i;				   // 7
		break_();
	}
		
	public static void test_jni_instance_method_calls_subclass(){
		SubMixedJava obj = new SubMixedJava();
		int a = obj.native_callback(); // 1
		int b = obj.i;				   // 1
		break_();
	}
	
	public static void test_jni_instance_method_calls_shared_method_id(){
		MixedJava obj1 = new MixedJava();
		SubMixedJava obj2 = new SubMixedJava();
		native_callback_2(obj1, obj2); 
		int a = obj1.i;					   // 1
		int b = obj2.i;				   	   // 1
		break_();
	}
	
	// test group 2
	
	public native void native_callbackA();
	
	public void callbackA(int val, MixedJava obj){
		this.i = 10;
		obj.i = obj.i + val;
	}
	
	public static void test_jni_instance_method_calls_args(){
		MixedJava obj = new MixedJava();
		obj.native_callbackA();
		int a = obj.i;				// 11
		break_();
	}
	
	//
	// JNI Nonvirtual Instance Method Calls
	//
	
	public native int native_non_virtual_callback();
	
	public static void test_jni_non_virtual_instance_method_call(){
		SubMixedJava obj = new SubMixedJava();
		obj.native_non_virtual_callback();
		int a = obj.i;
		break_();
	}
	
	//
	// JNI Static Method Calls
	//
	
	public static native int native_static_callback();
	public static native MixedJava native_static_callback_2();
	
	public static int static_callback(int val){
		return val * 2;
	}
	
	public static MixedJava static_callback2(){
		MixedJava obj = new MixedJava();
		obj.i = 7;
		return obj;
	}
	
	public static void test_jni_static_method_call(){
		int a = native_static_callback();
		break_();
	}
	
	public static void test_jni_static_method_call_return_obj(){
		MixedJava obj = native_static_callback_2();
		int a = obj.i;
		break_();
	}
	
	
	
	
	
	
	
	
	
	//
	// Instance Method Calls
	//
	
	static void test_instance_method_calls(){
		A i = new A();
		A j = new B();
		B k = new B();
		int a = i.f();
		int b = j.f();	
		int c = k.f();	
		int d = i.g();
		int e = j.g();
		int f = k.g();
		break_();
	}
	
	//
	// Static Method Calls
	//
	
	static void test_static_method_calls_0(){
		int a = A.static_f();
		int b = B.static_f();	
		int c = A.static_g();
		int d = B.static_g();
		break_();
	}
	
	static void test_static_method_calls_1(){
		A i = new A();
		A j = new B();
		B k = new B();
		int a = i.static_f();
		int b = j.static_f();	
		int c = k.static_f();	
		int d = i.static_g();
		int e = j.static_g();
		int f = k.static_g();
		break_();
	}
	
	//
	// Specialinvoke Method Calls
	//
	
	static void test_special_invoke_0(){
		A i = new B();
		int a = i.h();
		break_();
	}
	
	static void test_special_invoke_1(){
		B obj = new B();
		int a = obj.i;
		break_();
	}
	
	//
	// MISC
	//
	
	public static void main(String args[]){
		test_jni_non_virtual_instance_method_call();
	}
	
	public static void break_(){
		/* Add an additional basic block to the function, so the results
		   of the last statements are not deleted, when returning.
		*/
	}
	
		
	static {
		System.loadLibrary("MixedJava");
	}
	
	
}

class SubMixedJava extends MixedJava {
	int j = 1;
	public int callback(int val){
		this.i = 2;
		return this.i;
	}
}

class A {	
	int i;
	A(){
		i = 3;
	}
	int f(){ return 0; }
	int g(){ return 2; }
	int h(){ return 3; }
	
	static int static_f(){ return 0; }
	static int static_g(){ return 2; }
}

class B extends A {
	B(){
		super();
		i++;
	}
	int f(){ return 1; }
	int h(){ return super.h(); }
	static int static_f(){ return 1; }
}