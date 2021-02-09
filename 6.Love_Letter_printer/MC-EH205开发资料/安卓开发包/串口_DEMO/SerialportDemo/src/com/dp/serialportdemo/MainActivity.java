package com.dp.serialportdemo;



import com.dp.dp_serialportlist.Serialport_Factory;
import com.dp.dp_serialportlist.Serialport_Factory2;
import com.dp.serialportdemo.util.SharedPreferencesUtil;
import com.dp.serialportdemo.util.Toast_Util;
import com.szzk.percent.support.PercentRelativeLayout;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
public class MainActivity extends Activity {

	private PercentRelativeLayout set_prl,testprint_prl,testserialport_prl;
	public static Serialport_Factory sf;
	private Handler mhandler;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.activity_main);
		mhandler=new MHandler();
		sf=Serialport_Factory.getSerialport_Factory(this,mhandler);
		SharedPreferencesUtil.initPreferences(this);
		set_prl=(PercentRelativeLayout)findViewById(R.id.set_prl);
		set_prl.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View arg0) {
				Intent intent=new Intent(MainActivity.this,SetActivity.class);
				startActivity(intent);
			}
		});
		testserialport_prl=(PercentRelativeLayout)findViewById(R.id.testserialport_prl);
		testprint_prl=(PercentRelativeLayout)findViewById(R.id.testprint_prl);
		testprint_prl.setOnClickListener(new OnClickListener() {			
			@SuppressWarnings("static-access")
			@Override
			public void onClick(View arg0) {				
				
				if(!sf.isConnection())
				{
					Toast_Util.ToastString(getApplicationContext(), R.string.serial_port_does_not_open); //����û�д�
					return;
				}
				Intent intent=new Intent(MainActivity.this,NormalPrintActivity.class);
				startActivity(intent);
			}
		});
		
		testserialport_prl.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View arg0) {
				String baudrate=SharedPreferencesUtil.getbaud_rate();
				String com_name=SharedPreferencesUtil.getcom_name();
				boolean isopen=false;
				if(!baudrate.equals("")&&!com_name.equals(""))
				{
					isopen=sf.OpenPort(com_name, baudrate);
				}else {
					isopen=sf.OpenPort("ttyS0", "115200");  
				}	
				if(isopen)
				{
					Toast_Util.ToastString(getApplicationContext(), R.string.connected);
				}else {
					Toast_Util.ToastString(getApplicationContext(), R.string.unconnected);
				}
			}
		});	
	}
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		sf.Disconnect();
	}		
	 @SuppressLint("HandlerLeak")
		class MHandler extends Handler {
				@Override
				public void handleMessage(Message msg) {
					super.handleMessage(msg);			
					switch (msg.what) {						
						
						case Serialport_Factory.PAPER_STATE:{//检查缺纸状态
							boolean p=msg.getData().getBoolean("PAPER_STATE");
							if(p)
							{
								Toast_Util.ToastString(getApplicationContext(), "打印机有纸");
							}else {
								Toast_Util.ToastString(getApplicationContext(), "打印机缺纸");
							}
							break;
						}
					}
				}
			}
	
	
}
