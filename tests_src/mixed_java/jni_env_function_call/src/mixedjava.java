class MixedJava {
	
	static {
		System.loadLibrary("MixedJava");
	}
	
	native static int get_version();

	public static void main(String[] args) {
		int version = get_version();
	}
	
}