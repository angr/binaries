class MixedJava {

    public static void basic_multiarray_ops(){
        double[][] m = new double[3][3];
        for(int i=0; i<3; i++) {
            for(int j=0; j<3; j++) {
                m[i][j] = i*j;
            }
        }
        double res = m[2][2];
        if (true) {
            System.out.println("Done");
        }
        else {
            System.out.println("Never called");
        }
    }

    public static void main(String[] args) {
        basic_multiarray_ops();
    }
}

