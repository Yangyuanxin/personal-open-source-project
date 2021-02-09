package com.dp.usb_printdemo;

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

	public final static String PID = "PID";// ��ӡ��PID
	public final static String VID = "VID";//��ӡ��VID
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
	public static int getPID() {
		int mpid = preferences.getInt(PID, 0);
		return mpid;
	}

	/**
	 * ����PID
	 * @return
	 */
	public static void savePID(int mpid) {
		Editor editor = preferences.edit();
		editor.putInt(PID, mpid);
		editor.commit();
	}	

	/**
	 * ��ȡVID
	 * @return
	 */
	public static int getVID() {
		int mvid = preferences.getInt(VID, 0);
		return mvid;
	}

	/**
	 * ����VID
	 * @return
	 */
	public static void saveVID(int mvid) {
		Editor editor = preferences.edit();
		editor.putInt(VID, mvid);
		editor.commit();
	}
}
