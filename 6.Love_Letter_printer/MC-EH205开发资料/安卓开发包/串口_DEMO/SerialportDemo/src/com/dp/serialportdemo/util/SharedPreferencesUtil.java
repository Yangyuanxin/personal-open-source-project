package com.dp.serialportdemo.util;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.preference.PreferenceManager;

/**
 * ��ѡ����Ϣ
 * @author huningjun
 * 2015-7-1
 */
public class SharedPreferencesUtil {

	public final static String COMNAME = "com_name";// ��������
	public final static String BAUDRATE = "baud_rate";// ������
	/*
	 * ��ѡ�����Ϣ
	 */
	public static SharedPreferences preferences;
	public static void initPreferences(Context context) {
		if(preferences==null)
		{
			preferences = PreferenceManager.getDefaultSharedPreferences(context);
		}
		
	}

	/**
	 * ��ȡ��������
	 * @return
	 */
	public static String getcom_name() {
		String com_name = preferences.getString(COMNAME, "");
		return com_name;
	}

	/**
	 * ���洮������
	 * @return
	 */
	public static void savecom_name(String com_name) {
		Editor editor = preferences.edit();
		editor.putString(COMNAME, com_name);
		editor.commit();
	}	

	/**
	 * ��ȡ������
	 * @return
	 */
	public static String getbaud_rate() {
		String baud_rate = preferences.getString(BAUDRATE, "");
		return baud_rate;
	}

	/**
	 * ���沨����
	 * @return
	 */
	public static void savebaud_rate(String baud_rate) {
		Editor editor = preferences.edit();
		editor.putString(BAUDRATE, baud_rate);
		editor.commit();
	}
	
}
