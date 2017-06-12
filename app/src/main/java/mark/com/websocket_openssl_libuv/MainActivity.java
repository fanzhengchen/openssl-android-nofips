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
    private WebSocketClient mClient;

    TextView mTextView;
    TextView mBinaryTextView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mTextView = (TextView) findViewById(R.id.sample_text);
        mBinaryTextView = (TextView) findViewById(R.id.send_binary_text);

        mTextView.setText(System.getenv("os.arch") + " " + Build.DEVICE);


        mTextView.setOnClickListener(this);
        mBinaryTextView.setOnClickListener(this);

        getClient().connect(WS_URI);
    }


    @Override
    public void onClick(View v) {
        int id = v.getId();
        if (id == R.id.sample_text) {
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
