import mylib.MyLib;

class B {

    private int a;

    B(int a){
        this.a = a;
    }
}

class MixedJava {

    public static void testGetterAndSetterConcrete(MyLib lib){
        int a = lib.getMyInt();
        lib.setMyInt(1);
        char c = lib.getMyChar();
        lib.setMyChar('c');
        short d = lib.getMyShort();
        lib.setMyShort((short)1);
        long e = lib.getMyLong();
        lib.setMyLong(2L);
        float f = lib.getMyFloat();
        lib.setMyFloat(1.5f);
        double g = lib.getMyDouble();
        lib.setMyDouble(1.5);
        String h = lib.getMyString();
        lib.setMyString("Hello!");
        int[] i = lib.getMyArray();
        lib.setMyArray(new int[]{1,2,3});
        Object l = lib.getMyObject();
        lib.setMyObject(new B(1));
    }

    public static void main(String[] args) {

    }
}
