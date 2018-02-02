package com.example.phamh.factoryodroid;


import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {
    private RS485 m_rs485 = new RS485("/dev/ttyS2", 115200);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        m_rs485.handler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
                if (msg.what == RS485.HAD_DATA) {
                    byte[] data = (byte[]) (msg.obj);
                    Log.i("RS485_HAD_DATA", String.valueOf(data.length));
                } else if (msg.what == RS485.HAD_STATUS) {
                    String str = (String) msg.obj;
                    Log.i("RS485_STATUS", str);
                } else if (msg.what == RS485.HAD_ERROR) {
                    String str = (String) msg.obj;
                    Log.i("RS485_ERROR", str);
                }
            }
        };
        m_rs485.setPriority(Thread.NORM_PRIORITY);
        m_rs485.start();
        Button mButton = findViewById(R.id.button);
        mButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                byte[] msg = new byte[5];
                msg[0] = 'a';
                msg[1] = 'b';
                msg[2] = 'c';
                msg[3] = 'd';
                msg[4] = 'e';
                m_rs485.sendData(msg);
            }
        });
    }

}
