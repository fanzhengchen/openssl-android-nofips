package mark.com.websocket_openssl_libuv;

/**
 * Created by mark on 17-6-12.
 */

public interface WebSocketListener {

    void onTextMessage(String message);

    void onBinaryMessage(byte[] bytes);

    void onClose();

    void onError();

    void onConnect();
}
