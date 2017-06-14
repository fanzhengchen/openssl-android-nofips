package mark.com.websocket_openssl_libuv;

import com.orhanobut.logger.Logger;

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

    public native void sendBytes(byte[] bytes);

    public native void disconnect();

    public void onTextMessage(String message) {
        System.out.println(message);

        Logger.d(message);
        if (mSocketListener != null) {
            mSocketListener.onTextMessage(message);
        }
    }
}
