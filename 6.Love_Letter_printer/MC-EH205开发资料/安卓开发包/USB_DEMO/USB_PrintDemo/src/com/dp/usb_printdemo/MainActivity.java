package com.dp.usb_printdemo;

import java.util.HashMap;
import java.util.Iterator;

import com.dp.dp_usblist.USBFactory;
import com.dp.usb_printdemo.NormalPrintActivity.onGetActivit;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

public class MainActivity extends Activity {

	private static String TAG = "MainActivity";
	private final int CONNECTRESULT= 001;
	private LinearLayout linearLayoutUSBDevices;
	private Button search_print_bt,print_bt;
    private TextView usbconnt_tv;
	
	private UsbManager mUsbManager ;
	private HashMap<String, UsbDevice> deviceList;
	private Iterator<UsbDevice> deviceIterator ;
	public static USBFactory usbfactory;
	private Handler mHandler;
	private boolean connectstate=false;
	public static boolean ispage=false;
	private Activity normalprintactivity=null;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.fragment_main);
		mHandler=new MHandler();
		SharedPreferencesUtil.initPreferences(this);
		usbfactory=USBFactory.getUsbFactory(mHandler);
		usbconnt_tv=(TextView)findViewById(R.id.usbconnt_tv);
		search_print_bt=(Button)findViewById(R.id.search_print_bt);
		print_bt=(Button)findViewById(R.id.print_bt);
		linearLayoutUSBDevices = (LinearLayout) findViewById(R.id.linearLayoutUSBDevices);
		mUsbManager = (UsbManager) getSystemService(Context.USB_SERVICE);
	
		search_print_bt.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
//				if(!usbfactory.connectstate)
//				{					
					SearchUSB();	
//				}else {
//					Toast_Util.ToastString(getApplicationContext(), R.string.printer_connected); //打印机已连接
//				}
			}
		});
	
		print_bt.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
//				if(USBFactory.connectstate)
//				{					
					Intent intent=new Intent(MainActivity.this,NormalPrintActivity.class);
					startActivity(intent);		
//				}else {
//					Toast_Util.ToastString(getApplicationContext(), R.string.unconnected); //打印机已连接
//				}
			}
		});
	}
	//搜索USB设备search USB
	@SuppressLint("RtlHardcoded")
	private void SearchUSB() {
		isconnecting=true;
		deviceList = mUsbManager.getDeviceList();
		deviceIterator = deviceList.values().iterator();		
		linearLayoutUSBDevices.removeAllViews();
		if (deviceList.size() > 0) {
			// 初始化选择对话框布局，并添加按钮和事件
			while (deviceIterator.hasNext()) { // 这里是if不是while，说明我只想支持一种device
				final UsbDevice device = deviceIterator.next();
				device.getInterfaceCount();
				Button btDevice = new Button(
						linearLayoutUSBDevices.getContext());
				btDevice.setLayoutParams(new LayoutParams(
						LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
				btDevice.setGravity(android.view.Gravity.CENTER_VERTICAL
						| Gravity.LEFT);
				btDevice.setText("VID:"+device.getVendorId()+" ,  PID:"+device.getProductId());
				btDevice.setOnClickListener(new OnClickListener() {
					public void onClick(View v) {
						
						PendingIntent mPermissionIntent = PendingIntent.getBroadcast(MainActivity.this, 0, new Intent(getApplicationInfo().packageName), 0);
						if (!mUsbManager.hasPermission(device)) {
							mUsbManager.requestPermission(device,mPermissionIntent);
						} else {
							connectstate=usbfactory.connectUsb(mUsbManager, device);
							  if(connectstate)
							  {
								  mHandler.removeMessages(2);
								  VID=device.getVendorId()+"";
								  PID=device.getProductId()+"";
								  SharedPreferencesUtil.savePID(device.getProductId());
								  SharedPreferencesUtil.saveVID(device.getVendorId()); //保存连接成功打印机的PID和VID
								  linearLayoutUSBDevices.removeAllViews();
							  }
							  isconnecting=false;
							  mHandler.removeMessages(CONNECTRESULT);
							  mHandler.sendEmptyMessageDelayed(CONNECTRESULT, 1000);
						}
					}
				});
				
				linearLayoutUSBDevices.addView(btDevice);
			}
		}
		
		NormalPrintActivity.setOnGetActivit(new onGetActivit() {			
			@Override
			public void onbackactivity(Activity at) {
				// TODO Auto-generated method stub
				normalprintactivity=at;
			}
		});
	
		
	}
	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		normalprintactivity=null;
		mHandler.sendEmptyMessageDelayed(2, 500);
	}
	private boolean accredit=false;
	private boolean isconnecting=false;//是否正在搜索连接中
	private boolean isfind=false;//是否找到对应打印机
	private String VID,PID;
	//自动搜索并连接USB设备
	@SuppressLint("RtlHardcoded")
	private void AutomaticConnection() {
		int mvid=SharedPreferencesUtil.getVID();//获取上次连接成功打印机的VID
		int mpid=SharedPreferencesUtil.getPID();//获取上次连接成功打印机的PID
		if(isconnecting||usbfactory.connectstate||mvid==0)
		{
			return;
		}
		isfind=false;
		deviceList = mUsbManager.getDeviceList();
		deviceIterator = deviceList.values().iterator();		
		linearLayoutUSBDevices.removeAllViews();
		if (deviceList.size() > 0) {
			// 初始化选择对话框布局，并添加按钮和事件
			while (deviceIterator.hasNext()) { // 这里是if不是while，说明我只想支持一种device
				final UsbDevice device = deviceIterator.next();
				device.getInterfaceCount();
				int vid=device.getVendorId();
				int pid=device.getProductId();
			
//				if(vid==4070&&pid==33054)//判断是不是打印机编号
				Log.e("============", mvid+"======VID======"+vid);
				Log.e("============", mpid+"======PID======"+pid);
				if(vid==mvid&&pid==mpid)//判断是不是打印机编号
//				if(vid==4070&&pid==33054)//判断是不是打印机编号
				{
					isfind=true;
			    	PendingIntent mPermissionIntent ;
			    	if(normalprintactivity!=null)
			    	{
			    		 mPermissionIntent = PendingIntent.getBroadcast(normalprintactivity, 0, new Intent(getApplicationInfo().packageName), 0);
			    	}else {
			    		 mPermissionIntent = PendingIntent.getBroadcast(MainActivity.this, 0, new Intent(getApplicationInfo().packageName), 0);
					}
					if (!mUsbManager.hasPermission(device)) {
						if(!accredit)
						{
							mUsbManager.requestPermission(device,mPermissionIntent);
							accredit=true;
						}
						 mHandler.sendEmptyMessageDelayed(CONNECTRESULT, 800);
					} else {
						  accredit=false;
						  isconnecting=true;
						  connectstate=usbfactory.connectUsb(mUsbManager, device);
						  isconnecting=false;
						  if(connectstate)
						  {
							  VID=mvid+"";
							  PID=mpid+"";
							  mHandler.removeMessages(2);
						  }else 
						  mHandler.removeMessages(CONNECTRESULT);
						  mHandler.sendEmptyMessageDelayed(CONNECTRESULT, 1000);
					}
				}
			}
			if(!isfind)//没有搜索到对应打印机继续搜索
			{
				 mHandler.removeMessages(2);
				 mHandler.sendEmptyMessageDelayed(2, 1000);
			}			
		}
	}
	
	 @SuppressLint("HandlerLeak")
	class MHandler extends Handler {
			@SuppressLint("HandlerLeak") 
			@Override
			public void handleMessage(Message msg) {
				switch (msg.what) {
				case CONNECTRESULT: {
					if(usbfactory!=null)
					{
						if(usbfactory.getconnectstate())
						{
							 usbfactory.is_connectdevice();
							 usbconnt_tv.setText("=已连接   ["+"VID:"+VID+",PID:"+PID+"]");
							 mHandler.removeMessages(CONNECTRESULT);
							 mHandler.sendEmptyMessageDelayed(CONNECTRESULT, 2000);
						}else {
							 usbconnt_tv.setText("=未连接");
						     linearLayoutUSBDevices.removeAllViews();
						     mHandler.removeMessages(2);
							 mHandler.sendEmptyMessageDelayed(2, 500);
						}
					}
					
					break;
				}
				case 2:{
					AutomaticConnection();//自动连接
					break;
				}				
				case USBFactory.CHECKPAGE_RESULT:{
						if(msg.getData().getString("state").equals("1"))
						{
							ispage=true;
							Toast_Util.ToastString(getApplicationContext(), "printer has paper");//打印机有纸
							
						}else {
							ispage=false;
							Toast_Util.ToastString(getApplicationContext(), "printer has no paper");//打印机没有纸
						}
					break;
				}
				
				}
			}
		}
	 
		
}
