class MixedJava {
		
	native static boolean native_boolean(boolean p0);
	native static byte native_byte(byte p0);
	native static char native_char(char p0);
	native static short native_short(short p0);
	native static int native_int(int p0);
	native static long native_long(long p0);
	
	public static void test_boolean(){		
		boolean l0 = true;
		// false
		l0 = native_boolean(l0);
		// true
		l0 = native_boolean(l0);
		// false
		l0 = native_boolean(l0);
		// true
		l0 = native_boolean(l0);
		break_();
	}
	
	public static void test_byte(){
		byte b0, b1, b2;
		// (10 + 10) + 10 = 30
		b0 = 10;
		b1 = native_byte(b0);
		b2 = (byte) (b0 + b1);
		// (-128 + -128) + -128 = -128 mod 256
		b0 = -128;
		b1 = native_byte(b0);
		b2 = (byte) (b0 + b1);
		// (0 + 0) + 0 = 0
		b0 = 0;
		b1 = native_byte(b0);
		b2 = (byte) (b0 + b1);
		break_();
	}
	
	public static void test_char(){
		char c0, c1, c2;
		// (10 + 1) + 10 = 21
		c0 = 10;
		c1 = native_char(c0);
		c2 = (char) (c0 + c1);
		// (0x10000 - 1) + 1 = 0x10000 = 0 mod 2**16
		c0 = 0x10000-1;
		c1 = native_char(c0);
		// (0 + 1) + 1
		c0 = 0;
		c1 = native_char(c0);
		c2 = (char) (c0 + c1);
		break_();
	}
	
	public static void test_short(){
		short s0, s1, s2;
		// (0x4000 / 4) = 0x1000
		s0 = 0x4000;
		s1 = native_short(s0);
		// (-0x4000 / 4) = -0x1000 = 0x10fff
		s0 = -0x4000;
		s1 = native_short(s0);
		// (32 / 4) + 3 = 11
		s0 = 32;
		s1 = native_short(s0);
		s2 = (short) (s1 + 3);
		// (0 / 4) = 0
		s0 = 0;
		s1 = native_short(s0);
		break_();
	}
	
	public static void test_int(){
		int i0, i1, i2;
		// -10
		i0 = 10;
		i1 = native_int(i0);
		// -0
		i0 = 0;
		i1 = native_int(i0);
		// -0x7fffffff => 0x80000001
		i0 = 0x7fffffff;
		i1 = native_int(i0);
		// -0x80000000 - 1 => 0x7fffffff
		i0 = -2147483648;
		i1 = i0 - 1;
		break_();
	}
	
	public static void test_long(){
		long l0, l1, l2;
		// -3 + 2 = -1 => 0xffffffffffffffff
		l0 = -3;
		l1 = native_long(l0);
		//  -1 + 2 = 1
		l0 = -1;
		l1 = native_long(l0);
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
