class Fauxware {
	
	static { System.loadLibrary("fauxware"); }
	private static native boolean authenticate(String password);
	
	private static void accepted(){
		System.out.println("Welcome to the admin console, trusted user!");
	}
	
	private static void rejected(){
		System.out.println("Go away!");
		System.exit(0);
	}

	public static void main(String[] args){
	    if (args.length != 1){
	        System.out.println("Usage: fauxware 'password'");
	        System.exit(0);
	    }
		String password = args[0];
        // check credentials
		boolean authed = authenticate(password);
		if (authed) accepted();
		else rejected();
	}
}