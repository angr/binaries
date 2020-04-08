package mylib;

public class MyLib {

    private int myInt;
    private char myChar;
    private short myShort;
    private long myLong;
    private float myFloat;
    private double myDouble;
    private String myString;
    private int[] myArray;
    private Object myObject;

    public MyLib(
            int myInt,
            char myChar,
            short myShort,
            long myLong,
            float myFloat,
            double myDouble,
            String myString,
            int[] myArray,
            Object myObject
    ) {
        this.myInt = myInt;
        this.myChar = myChar;
        this.myShort = myShort;
        this.myLong = myLong;
        this.myFloat = myFloat;
        this.myDouble = myDouble;
        this.myString = myString;
        this.myArray = myArray;
        this.myObject = myObject;
    }

    public int getMyInt() {
        return myInt;
    }

    public void setMyInt(int myInt) {
        this.myInt = myInt;
    }

    public char getMyChar() {
        return myChar;
    }

    public void setMyChar(char myChar) {
        this.myChar = myChar;
    }

    public short getMyShort() {
        return myShort;
    }

    public void setMyShort(short myShort) {
        this.myShort = myShort;
    }

    public long getMyLong() {
        return myLong;
    }

    public void setMyLong(long myLong) {
        this.myLong = myLong;
    }

    public float getMyFloat() {
        return myFloat;
    }

    public void setMyFloat(float myFloat) {
        this.myFloat = myFloat;
    }

    public double getMyDouble() {
        return myDouble;
    }

    public void setMyDouble(double myDouble) {
        this.myDouble = myDouble;
    }

    public String getMyString() {
        return myString;
    }

    public void setMyString(String myString) {
        this.myString = myString;
    }

    public int[] getMyArray() {
        return myArray;
    }

    public void setMyArray(int[] myArray) {
        this.myArray = myArray;
    }

    public Object getMyObject() {
        return myObject;
    }

    public void setMyObject(Object myObject) {
        this.myObject = myObject;
    }
}
