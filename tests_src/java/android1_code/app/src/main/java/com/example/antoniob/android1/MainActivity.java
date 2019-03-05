package com.example.antoniob.android1;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

import java.util.Random;

public class MainActivity extends AppCompatActivity {

    public final static String TAG = "ANDROID1";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        int a = 137;
        int b = 251;
        Random rr = new Random();
        int r = rr.nextInt();
        Log.d(TAG, String.valueOf(a*b*r));
    }
}
