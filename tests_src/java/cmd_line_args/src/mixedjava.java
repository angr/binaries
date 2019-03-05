class MixedJava {
	
	public static void main(String[] args) throws Exception {
		if(args[0].equals("secret_value"))
			System.out.println("Suuuuup!");
		else
			System.out.println("Nope.");
	}

	public static void break_(){
	/* Add an additional basic block to the function, so the results
	 * of the last statements are not deleted when returning.
	 */
	}
	
}
