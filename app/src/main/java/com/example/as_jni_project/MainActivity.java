package com.example.as_jni_project;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.example.as_jni_project.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'as_jni_project' library on application startup.
    static {
        System.loadLibrary("as_jni_project");
    }

    private ActivityMainBinding binding;

    private final static String TAG = MainActivity.class.getSimpleName();

    //交互操作 JNI
    public native void testArrayAction(int count,String textIndo,int[] ints,String[] strs);
    public native void putObject(Student student,String str);//传递引用类型，传递对象
    public native void insertObject();//凭空创建java对象



    //点击事件：操作testArrayAction函数
    public void test01(View view){
        int[] ints = new int[]{1,2,3,4,5,6};
        String[] strs = new String[]{"李小龙","李连杰","李元霸"};
        testArrayAction(99,"你好",ints,strs);
        for (int aInt:ints){
            Log.d(TAG,"test01:aint---"+aInt);
        }
    }


    public void test02(View view){
        Student student = new Student();
        student.name = "史泰龙";
        student.age = 88;
        putObject(student, "九阳神功");
    }


    public void test03(View view){
        insertObject();
    }

    public void test04(View view){

    }

    public void test05(View view){

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
    }

    /**
     * A native method that is implemented by the 'as_jni_project' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

}