package com.dp.usb_printdemo;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.preference.PreferenceManager;

/**
 * 首选项信息
 * @author huningjun
 * 2015-7-1
 */
public class SharedPreferencesUtil {

	public final static String PID = "PID";// 打印机PID
	public final static String VID = "VID";//打印机VID
	/*
	 * 首选项保存信息
	 */
	public static SharedPreferences preferences;
	public static void initPreferences(Context context) {
		if(preferences==null)
		{
			preferences = PreferenceManager.getDefaultSharedPreferences(context);
		}
		
	}
	
	/**
	 * 获取串口名称
	 * @return
	 */
	public static int getPID() {
		int mpid = preferences.getInt(PID, 0);
		return mpid;
	}

	/**
	 * 保存PID
	 * @return
	 */
	public static void savePID(int mpid) {
		Editor editor = preferences.edit();
		editor.putInt(PID, mpid);
		editor.commit();
	}	

	/**
	 * 获取VID
	 * @return
	 */
	public static int getVID() {
		int mvid = preferences.getInt(VID, 0);
		return mvid;
	}

	/**
	 * 保存VID
	 * @return
	 */
	public static void saveVID(int mvid) {
		Editor editor = preferences.edit();
		editor.putInt(VID, mvid);
		editor.commit();
	}
}
