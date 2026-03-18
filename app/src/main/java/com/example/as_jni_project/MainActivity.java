package com.example.as_jni_project;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import org.fmod.FMOD;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class MainActivity extends AppCompatActivity {

    private static final int MODE_NORMAL = 0; // 正常
    private static final int MODE_LUOLI = 1; //
    private static final int MODE_DASHU = 2; //
    private static final int MODE_JINGSONG = 3; //
    private static final int MODE_GAOGUAI = 4; //
    private static final int MODE_KONGLING = 5; //
    private String audioFilePath; // 真实文件路径

    private String path;

    static {
        System.loadLibrary("native-lib");
    }

    private void copyAssetToFiles(String assetName) {
        try {
            InputStream in = getAssets().open(assetName);
            File outFile = new File(getFilesDir(), assetName);
            audioFilePath = outFile.getAbsolutePath();

            if (!outFile.exists()) {
                OutputStream out = new FileOutputStream(outFile);
                byte[] buffer = new byte[1024];
                int read;
                while ((read = in.read(buffer)) != -1) {
                    out.write(buffer, 0, read);
                }
                out.close();
            }
            in.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        copyAssetToFiles("derry.mp3");

        FMOD.init(this);

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        FMOD.close();
    }

    public void onFix(View view) {
        Log.d("cyr","打印一下audioFilePath----"+audioFilePath);
        int id = view.getId();
        if (id == R.id.btn_normal) {
            voiceChangeNative(MODE_NORMAL, audioFilePath);
        } else if (id == R.id.btn_luoli) {
            voiceChangeNative(MODE_LUOLI, audioFilePath);
        } else if (id == R.id.btn_dashu) {
            voiceChangeNative(MODE_DASHU, audioFilePath);
        } else if (id == R.id.btn_jingsong) {
            voiceChangeNative(MODE_JINGSONG, audioFilePath);
        } else if (id == R.id.btn_gaoguai) {
            voiceChangeNative(MODE_GAOGUAI, audioFilePath);
        } else if (id == R.id.btn_kongling) {
            voiceChangeNative(MODE_KONGLING, audioFilePath);
        }
    }

    public void playerEnd(String mes){
        Toast.makeText(this,mes,Toast.LENGTH_LONG).show();
    }

    public native void voiceChangeNative(int modelNormal,String path);


}