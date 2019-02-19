package exceptions1;

import java.util.Random;

public class Main {
	
	public static void m1(){
		System.out.println("0");
		Random rand = new Random();
		if(rand.nextInt()%2==0){
			System.out.println("11");
		}else{
			System.out.println("12");
		}
		
		try{
			rand = new Random();
			if(rand.nextInt()%2==0){
				System.out.println("1");
				if(rand.nextInt()%2==0){
					throw new Exception();
				}
			}else{
				System.out.println("2");
			}
		}catch(Exception e){
			System.out.println("3");
		}
		System.out.println("4");
	}

	public static void main(String[] args) {
		m1();
	}

}
