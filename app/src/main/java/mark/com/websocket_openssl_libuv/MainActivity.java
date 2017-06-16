package mark.com.websocket_openssl_libuv;

import android.app.Activity;
import android.os.Build;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.example.WebSocket;

import org.w3c.dom.Text;

public class MainActivity extends Activity implements View.OnClickListener {


    private static final String WS_URI = "ws://172.16.14.115:20000/webSocket";

    private WebSocket mWebSocket = new WebSocket();

    TextView mTextView;
    TextView mBinaryTextView;
    TextView mPlainText;
    TextView mCloseView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mTextView = (TextView) findViewById(R.id.connect);
        mBinaryTextView = (TextView) findViewById(R.id.send_binary_text);
        mPlainText = (TextView) findViewById(R.id.send_plain_text);
        mCloseView = (TextView) findViewById(R.id.close);


        mPlainText.setOnClickListener(this);
        mTextView.setOnClickListener(this);
        mBinaryTextView.setOnClickListener(this);
        mCloseView.setOnClickListener(this);

    }


    @Override
    public void onClick(View v) {
        int id = v.getId();
        if (id == R.id.connect) {
            getClient().connect(WS_URI);
        } else if (id == R.id.send_plain_text) {
            getClient().sendPlainText("wwwwwwwwwwwwwwwwwwwwwwwww text text");
        } else if (id == R.id.send_binary_text) {
            byte[] bytes = "bbbbbbbbbbbbbbbbbbbbbbb".getBytes();
            getClient().sendBytes(bytes);
        } else if (id == R.id.close) {
            getClient().close();
        }
    }


    public WebSocket getClient() {
        if (mWebSocket == null) {
            mWebSocket = new WebSocket();
        }
        return mWebSocket;
    }
}
