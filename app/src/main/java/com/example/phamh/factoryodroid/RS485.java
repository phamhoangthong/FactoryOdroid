package com.example.phamh.factoryodroid;

import android.os.Handler;
import android.os.Message;

/**
 * Created by phamh on 2/2/2018.
 */

public class RS485 extends Thread {
    public static int HAD_DATA = 1;
    public static int HAD_ERROR = 2;
    public static int HAD_STATUS = 3;

    static {
        System.loadLibrary("native-lib");
    }

    public Handler handler;
    private int id;
    private boolean flag_run;

    public RS485(String port, int speed) {
        id = getIdRS485(port, speed);
        flag_run = false;
    }

    public void run() {
        flag_run = true;
        sendStatus(new String("Start module"));
        while (flag_run) {
            byte[] data = readDataRS485(id);
            if (data.length == 0) {
                sendError(new String("Error receive data"));
            } else {
                Message msg = handler.obtainMessage();
                msg.obj = data;
                msg.what = HAD_DATA;
                handler.sendMessage(msg);
            }
        }
    }

    public void f_stop() {
        flag_run = false;
        try {
            join();
            releaseRS485(id);
            sendStatus(new String("Stop module"));
        } catch (InterruptedException ex) {
            sendError(new String("Error stop"));
        }
    }

    public void sendData(byte[] data) {
        int rc = sendDataRS485(id, data);
        if (rc < 0) {
            sendError(new String("Error send data"));
        } else {
            sendStatus(new String("Sended data"));
        }
    }

    private void sendError(String str) {
        Message msg = handler.obtainMessage();
        msg.obj = str;
        msg.what = HAD_ERROR;
        handler.sendMessage(msg);
    }

    private void sendStatus(String str) {
        Message msg = handler.obtainMessage();
        msg.obj = str;
        msg.what = HAD_STATUS;
        handler.sendMessage(msg);
    }

    public native int getIdRS485(String port, int speed);

    public native byte[] readDataRS485(int id);

    public native int sendDataRS485(int id, byte[] data);

    public native int releaseRS485(int id);
}
