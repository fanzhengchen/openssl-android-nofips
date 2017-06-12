package mark.com.websocket_openssl_libuv;

/**
 * Created by mark on 17-6-12.
 */

public class WebSocketClient {


    WebSocketListener mSocketListener;

    static {
        System.loadLibrary("native-lib");
    }


    public native void connect(String uri);

    public native void send(String message);

    public native void send(byte[] bytes);

    public native void disconnect();

    private void onTextMessage(String message) {
        if (mSocketListener != null) {
            mSocketListener.onTextMessage(message);
        }
    }
}
