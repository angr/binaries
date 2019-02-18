class MixedJava {	
	
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
