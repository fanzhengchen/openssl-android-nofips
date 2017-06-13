package mark.com.websocket_openssl_libuv;

import android.app.Activity;
import android.os.Build;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import org.w3c.dom.Text;

public class MainActivity extends Activity implements View.OnClickListener {


    private static final String WS_URI = "ws://172.16.14.115:20000/webSocket";
    private WebSocketClient mClient = new WebSocketClient();

    TextView mTextView;
    TextView mBinaryTextView;
    TextView mPlainText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mTextView = (TextView) findViewById(R.id.sample_text);
        mBinaryTextView = (TextView) findViewById(R.id.send_binary_text);
        mPlainText = (TextView) findViewById(R.id.send_plain_text);

        mTextView.setText(System.getenv("os.arch") + " " + Build.DEVICE);

        mPlainText.setOnClickListener(this);
        mTextView.setOnClickListener(this);
        mBinaryTextView.setOnClickListener(this);

    }


    @Override
    public void onClick(View v) {
        int id = v.getId();
        if (id == R.id.sample_text) {
            getClient().connect(WS_URI);
        } else if (id == R.id.send_plain_text) {
            getClient().send("wwwwwwwwwwwwwwwwwwwwwwwww text text");
        } else if (id == R.id.send_binary_text) {
            byte[] bytes = "bbbbbbbbbbbbbbbbbbbbbbb".getBytes();
            getClient().sendBytes(bytes);
        }
    }


    public WebSocketClient getClient() {
        if (mClient == null) {
            mClient = new WebSocketClient();
        }
        return mClient;
    }
}
