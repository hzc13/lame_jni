package cn.itcast.lame;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import android.app.Activity;
import android.app.ProgressDialog;

public class MainActivity extends Activity {
	
	static {
		System.loadLibrary("hello");
	}
	
	private EditText wavName;
	private EditText mp3Name;
	private Button convertBtn;
	private Button versionBtn;
	private ProgressDialog pd;
	
	public native void convertToMp3(String wav, String mp3);
	public native String getVersion();

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		wavName = (EditText) findViewById(R.id.editText1);
		mp3Name = (EditText) findViewById(R.id.editText2);
		convertBtn = (Button) findViewById(R.id.btn1);
		versionBtn = (Button) findViewById(R.id.btn2);
		pd = new ProgressDialog(this);
	}
	
	public void convert(View v){
		final String wav = wavName.getText().toString();
		final String mp3 = mp3Name.getText().toString();
		pd.setMessage("×ª»»ÖÐ......");
		pd.show();
		new Thread(){
			public void run() {
				convertToMp3(wav, mp3);
				pd.dismiss();
			};
		}.start();
	}
	
	public void showVersion(View v){
		Toast.makeText(this, getVersion(), Toast.LENGTH_SHORT).show();
	}
}
