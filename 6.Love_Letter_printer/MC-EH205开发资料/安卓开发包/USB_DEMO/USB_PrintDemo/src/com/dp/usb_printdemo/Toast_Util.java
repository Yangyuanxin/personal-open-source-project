package com.dp.usb_printdemo;

import android.content.Context;
import android.view.Gravity;
import android.widget.Toast;

public class Toast_Util {
	public Toast_Util() {
		// TODO Auto-generated constructor stub
	}
	static Toast toast=null;
	public static void ToastString(Context context,String str){
        if(toast!=null)
        {
        	toast.cancel();
        }
		toast=Toast.makeText(context, str,Toast.LENGTH_SHORT);		
		toast.setGravity(Gravity.CENTER, 0, 0);
		toast.show();
	}
	public static void ToastString(Context context,int str){
        if(toast!=null)
        {
        	toast.cancel();
        }
		toast=Toast.makeText(context, str,Toast.LENGTH_SHORT);		
		toast.setGravity(Gravity.CENTER, 0, 0);
		toast.show();
	}
}
