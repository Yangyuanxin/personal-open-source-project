package com.dp.serialportdemo.util;

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

	public final static String COMNAME = "com_name";// 串口名称
	public final static String BAUDRATE = "baud_rate";// 波特率
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
	public static String getcom_name() {
		String com_name = preferences.getString(COMNAME, "");
		return com_name;
	}

	/**
	 * 保存串口名称
	 * @return
	 */
	public static void savecom_name(String com_name) {
		Editor editor = preferences.edit();
		editor.putString(COMNAME, com_name);
		editor.commit();
	}	

	/**
	 * 获取波特率
	 * @return
	 */
	public static String getbaud_rate() {
		String baud_rate = preferences.getString(BAUDRATE, "");
		return baud_rate;
	}

	/**
	 * 保存波特率
	 * @return
	 */
	public static void savebaud_rate(String baud_rate) {
		Editor editor = preferences.edit();
		editor.putString(BAUDRATE, baud_rate);
		editor.commit();
	}
	
}
