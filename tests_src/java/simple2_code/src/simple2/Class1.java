package simple2;

import java.util.RandomAccess;

import java.util.Random;

public class Class1 implements Interface1, Interface2{
	int ff1 = 4;
	private double ff2 = 3;
	Object ff3 = new Class2();

	public static void main(String[] args) {			
		System.out.println("START!");
		Class1 cc = new Class1();
		Class2 t;
		try {
			t = cc.doStuff("asd");
		} catch (Exception e) {
			e.printStackTrace();
		}
		System.out.println(String.valueOf(null));
		System.out.println("END!");
	}
	
	int unreachable(int a){
		Random rand = new java.util.Random();
		int  n = rand.nextInt(a) + 1;
		return n;
	}

	Class2 doStuff(String i1) throws Exception{
		int a,z1 = 0,z2,z3;
		int b[]  = new int[10];
		int c[][][] = new int[10][20][30];

		Object cc1,cc2,cc3,cc4,cc5,cc6,cc7,cc8;
		short cc9;
		char cc10;
		byte cc11;

		cc1 = null;
		cc2 = 1;
		cc3 = 1L;
		cc4 = 1.0f;
		cc5 = 1.0d;
		cc6 = false;
		cc7 = "aaa";
		cc8 = Class1.class;
		cc9 = 1;
		cc10 = 'a';
		cc11 = 1;

		cc5 = this.ff2;

		cc11 = (byte) c[1][2][3];
		if(cc1 instanceof Class1){
			cc10 = 'e';
		}

		switch(cc11){
		case 1:
			cc10 = 'a';
			break;
		case 2:
			cc10 = 'b';
			break;
		case 100:
			cc10 = 'c';
		case -3:
			cc10 = 'd';
			break;
		default:
			cc10 = 'c';
		}
		
		switch(z1){
		case 1:
			cc10 = 'a';
			break;
		case 2:
			cc10 = 'b';
			break;
		case 3:
			cc10 = 'c';
			break;
		default:
			cc10 = 'd';
		}


		try{
			throw(new Exception());
		}catch(Exception e){
			cc10 = 'd';
		}

		Random rand = new java.util.Random();
		int  n = rand.nextInt(50) + 1;
		a = 3;
		a += n*3;
		if(a > 37){
			z1 = 1;
		}else{
			z1 = 2;
		}
		z2 = z1;

		int r = b.length;

		synchronized(ff3){
			int	t = n*1&n*2*n*3+n*4-n*5/n*6&n*1^n*2|n*3%n*7;
		}

		boolean bb = rand.nextBoolean();
		if(bb && true || (false || !bb)){
			z1 = 1;
		}else{
			z1 = 2;
		}

		Interface1 ii1 = this;
		ii1.i1();
		Class1 ccc2 = (Class1) ii1;
		Class1 ccc3 = new Class1();

		System.out.println(String.valueOf(z2*r));
		return new Class2();
	}

	class Inner1 {
		public Inner1() {
			super();
			int a = 2;
		}
	}
	
	public void i2() {
		int c = 2;
	}

	public synchronized void i1() {
		int c = 1;
	}

}
