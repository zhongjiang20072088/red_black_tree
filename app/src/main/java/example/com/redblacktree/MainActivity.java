package example.com.redblacktree;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import android.view.View.OnClickListener;

public class MainActivity extends AppCompatActivity implements View.OnClickListener{

    private RBTreeView mTreeView;
    private Random mRandom;
    private int i;
    private List<Integer> mTrack = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

    // Example of a call to a native method
    //TextView tv = (TextView) findViewById(R.id.sample_text);
    //tv.setText(stringFromJNI());
        //tv.setText("ABAB");

        mTreeView = (RBTreeView) findViewById(R.id.tree);
        mRandom = new Random(37);
        findViewById(R.id.add).setOnClickListener(this);
        findViewById(R.id.delete).setOnClickListener(this);

    }

    @Override
    public void onClick(View v) {
        int random;
        switch (v.getId()) {
            case R.id.add:
                random = mRandom.nextInt(39);
                mTrack.add(random);
                mTreeView.insert(random);
                break;
            case R.id.delete:
                if (mTrack.size() == 0)
                    return;
                random = mRandom.nextInt(mTrack.size());
                if (random >= mTrack.size()) {
                    Log.d("zhongjiang", "delete " + random + " track size " + mTrack.size());
                    return;
                }
                int next = mTrack.get(random);
                Log.d("zhongjiang", "delete " + next);
                mTreeView.delete(next);
                mTrack.remove(random);
                break;
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    //public native String stringFromJNI();

    // Used to load the 'native-lib' library on application startup.
    /*static {
        try {
            System.loadLibrary("native-lib");
        } catch (Exception e) {
            e.printStackTrace();
        }

    }*/



    @Override
    protected void onDestroy() {
        super.onDestroy();
    }
}
