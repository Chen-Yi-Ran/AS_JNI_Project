package com.example.as_jni_project;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.example.as_jni_project.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'as_jni_project' library on application startup.
    static {
        System.loadLibrary("as_jni_project");
    }

    private ActivityMainBinding binding;

    public  static final  int A = 100;
    public static int age = 29;

    public String name = "Logan";//签名：Ljava/lang/String;

    //交互操作 JNI
    public native void changeName();
    public static native void changeAge();
    public native void callAddMethod();


    //写一个函数，给native调用
    public int add(int number1,int number2){
        return number1+number2+8;
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        //tv.setText(stringFromJNI());
        changeName();
        tv.setText(name);
        changeAge();
        tv.setText(""+age);
        callAddMethod();
    }

    /**
     * A native method that is implemented by the 'as_jni_project' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();


    public static native String StringFromJNI2();
}