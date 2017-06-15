package com.example;

public class WebSocket {

    static {
        System.loadLibrary("websocket");
    }

    public native void connect(String uri);

    public native void close();

    public native void sendPlainText(String text);

    public native void sendBytes(byte[] bytes);


}
