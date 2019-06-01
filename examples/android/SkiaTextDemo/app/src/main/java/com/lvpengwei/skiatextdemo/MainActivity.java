package com.lvpengwei.skiatextdemo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.lvpengwei.skiatext.SkiaView;

public class MainActivity extends AppCompatActivity {

    private static String TAG = "MainActivity";
    private SkiaView skiaView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        skiaView = findViewById(R.id.skia_view);
        skiaView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.d(TAG, "onClick");
                skiaView.flush();
            }
        });
    }
}
