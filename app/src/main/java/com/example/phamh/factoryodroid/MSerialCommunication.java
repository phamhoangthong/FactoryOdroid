package com.example.phamh.factoryodroid;

import android.os.Handler;
import android.os.Message;

/**
 * Created by phamh on 1/30/2018.
 */

public class MSerialCommunication {
    static {
        System.loadLibrary("native-lib");
    }

    private Handler m_handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            if(msg.what == 1) {

            }
        }
    };

    private long pointer;

    public void sendDataToHandler(char[] data) {
        int length = data.length;
        char[] buffer = new char[length];
        System.arraycopy(data,0,buffer,0, length);
        Message msg = m_handler.obtainMessage();
        msg.what = 1;
        msg.obj = buffer;
        m_handler.sendMessage(msg);
    }

    public MSerialCommunication(String port, int speed) {
        pointer = createSerial(port, speed);
    }

    public void sendData(char[] data) {
        sendToSerial(pointer, data);
    }

    public native long createSerial(String port, int speed);
    public native int sendToSerial(long obj, char[] data);
}
