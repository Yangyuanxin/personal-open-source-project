package com.dp.serialportdemo;

import com.dp.serialportdemo.util.SharedPreferencesUtil;
import com.dp.serialportdemo.util.Toast_Util;
import com.dp.serialportdemo.view.PopMenu;
import com.dp.serialportdemo.view.PopMenu.OnItemClickListener;
import com.szzk.percent.support.PercentRelativeLayout;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

public class SetActivity extends Activity {

	private PercentRelativeLayout input_prl,drivername_prl;
	public PopMenu versions,drivername_pm;
	private TextView baud_rate_et,drivername_tv;
	private Button save_set_bt;
	private ImageView back_iv;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setSoftInputMode( WindowManager.LayoutParams.SOFT_INPUT_STATE_HIDDEN);//一开始不会弹出软键盘需要点击文本框才会弹出软键盘
		setContentView(R.layout.activity_set);
		baud_rate_et=(TextView)findViewById(R.id.baud_rate_et);
		drivername_tv=(TextView)findViewById(R.id.drivername_tv);
		input_prl=(PercentRelativeLayout)findViewById(R.id.input_prl);
		drivername_prl=(PercentRelativeLayout)findViewById(R.id.drivername_prl);
		save_set_bt=(Button)findViewById(R.id.save_set_bt);
		back_iv=(ImageView)findViewById(R.id.back_iv);
		versions = new PopMenu();
		drivername_pm = new PopMenu();
		getbaud_rate();
		versions.setOnItemClickListener(new OnItemClickListener() {			
			@Override
			public void onItemClick(int index) {
				String str=versions.getItem(index);
				baud_rate_et.setText(str);	
			}
		});
		
		drivername_pm.setOnItemClickListener(new OnItemClickListener() {			
			@Override
			public void onItemClick(int index) {
				String str=drivername_pm.getItem(index);
				drivername_tv.setText(str);	
			}
		});
		input_prl.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View v) {				
				versions.showAsDropDown(v,getApplicationContext(),v.getWidth());				
			}
		});
		drivername_prl.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View v) {				
				drivername_pm.showAsDropDown(v,getApplicationContext(),v.getWidth());				
			}
		});
		save_set_bt.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View arg0) {
				String com_name=drivername_tv.getText().toString().trim();
				String baud_rate=baud_rate_et.getText().toString().trim();
				if(com_name.equals(""))
				{
					Toast_Util.ToastString(getApplicationContext(), R.string.please_enter_a_serial_port_name);//请输入串口名称
				}else {
					SharedPreferencesUtil.savecom_name(com_name);
					SharedPreferencesUtil.savebaud_rate(baud_rate);
					Toast_Util.ToastString(getApplicationContext(), R.string.saved_successfully);  //保存成功
					finish();
//					mApplication.closeSerialPort();					
				}				
			}
		});
		back_iv.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				finish();
			}
		});
	}
	
	
	private void getbaud_rate()
	{
		String baudrate=SharedPreferencesUtil.getbaud_rate();
		String comname=SharedPreferencesUtil.getcom_name();
		if(baudrate!=null)
		{
			if(baudrate.length()==0){
				baud_rate_et.setText("115200");
			}else {
				baud_rate_et.setText(baudrate);
			}
		}
		if(comname!=null)
		{
			if(comname.length()==0){
				drivername_tv.setText("ttySAC0");
			}else {
				drivername_tv.setText(comname);
			}
			
		}
		versions.clearItems();		
		versions.addItem("1200");
		versions.addItem("2400");
		versions.addItem("4800");
		versions.addItem("9600");
		versions.addItem("14400");
		versions.addItem("19200");
		versions.addItem("38400");
		versions.addItem("57600");
		versions.addItem("115200");
		versions.addItem("230400");
		versions.addItem("256000");
		versions.addItem("500000");
		versions.addItem("750000");
		versions.addItem("1125000");
		String dlist[]=MainActivity.sf.getAllDevices();//获取所有串口设备名称
		drivername_pm.clearItems();
		for(int i=0;i<dlist.length;i++)
		{
			drivername_pm.addItem(dlist[i]);
		}
	}
	
}
