package com.dp.serialportdemo.view;

import java.util.ArrayList;
import java.util.HashMap;

import com.dp.serialportdemo.R;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.BaseAdapter;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.PopupWindow;
import android.widget.TextView;

/**
 * �޸��ڣ�2015-6-17
 * 
 *  ���� ListView item �����Ӧʧ�ܣ�
 * 
 * @author 
 *
 */
public class PopMenu implements OnItemClickListener {
	public interface OnItemClickListener {
		public void onItemClick(int index);
	}
	private ArrayList<String> itemList;
	private PopupWindow popupWindow;
	private ListView listView;
	private OnItemClickListener listener;
	private LayoutInflater inflater;
	private LinearLayout pomenu_item_ll;
    private PopAdapter pa;
	HashMap<Integer,View> lmap = new HashMap<Integer,View>();
	private TextView tView;
	public PopMenu() {
		itemList = new ArrayList<String>();
		pa=new PopAdapter();
	}

	@SuppressLint("ResourceAsColor")
	@Override
	public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
		if (listener != null) {
			listener.onItemClick(position);			
			if(pomenu_item_ll==null)
			{
				tView=(TextView)view.findViewById(R.id.textView);
				pomenu_item_ll=(LinearLayout)view.findViewById(R.id.pomenu_item_ll);
//				pomenu_item_ll.setBackgroundColor(Color.parseColor("#2ECABF"));
//				tView.setTextColor(Color.WHITE);
			}else {
//				pomenu_item_ll.setBackgroundColor(0);
//				tView.setTextColor(R.color.pop_item_word);
				tView=(TextView)view.findViewById(R.id.textView);
				pomenu_item_ll=(LinearLayout)view.findViewById(R.id.pomenu_item_ll);
//				pomenu_item_ll.setBackgroundColor(Color.parseColor("#2ECABF"));
//				tView.setTextColor(Color.WHITE);
			}			
		}
		dismiss();
	}

	// ���ò˵�����������
	public void setOnItemClickListener(OnItemClickListener listener) {
		 this.listener = listener;
	}
	
	// ������Ӳ˵���
	public void addItems(String[] items) {
		for (String s : items)
			itemList.add(s);
	}
	// ������Ӳ˵���
	public void clearItems() {		
		itemList.clear();
	}

	// ������Ӳ˵���
	public void addItem(String item) {
		itemList.add(item);
	}
	public String getItem(int index)
	{
		return itemList.get(index);
	}

	// ����ʽ ���� pop�˵� parent ���½�
	public void showAsDropDown(View parent,Context context,int ww) {
		inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		View view = inflater.inflate(R.layout.popmenu, null);
		listView = (ListView) view.findViewById(R.id.listView);
		Log.e("================PopMenu=="+parent.getWidth(), "===================="+ww);
		listView.getLayoutParams().width=ww;
		listView.setAdapter(pa);
		listView.setOnItemClickListener(this);		
		popupWindow = new PopupWindow(view,LayoutParams.WRAP_CONTENT,  //��������Ҫ�Լ�ָ����ʹ�� WRAP_CONTENT ��ܴ�
				LayoutParams.WRAP_CONTENT);
//		popupWindow.showAtLocation(parent, Gravity.RIGHT, 30, -20);// ��������ʾ
		// �����Ϊ�˵��������Back��Ҳ��ʹ����ʧ�����Ҳ�����Ӱ����ı���
		popupWindow.setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
		popupWindow.showAsDropDown(parent);
		// ʹ��ۼ�
		popupWindow.setFocusable(true);
		// ����������������ʧ
		popupWindow.setOutsideTouchable(true);
		// ˢ��״̬
		popupWindow.update();
		
	}

	// ���ز˵�
	public void dismiss() {
		if(popupWindow!=null)
		{
			popupWindow.dismiss();	
		}
		
	}

	// ������
	private final class PopAdapter extends BaseAdapter {
		@Override
		public int getCount() {
			return itemList.size();
		}

		@Override
		public Object getItem(int position) {
			return itemList.get(position);
		}

		@Override
		public long getItemId(int position) {
			return position;
		}

		@Override
		public View getView(int position, View convertView, ViewGroup parent) {
			final ViewHolder holder;			
//			if (convertView == null) {
			if (lmap.get(position)==null) {
				convertView = inflater.inflate(R.layout.pomenu_item, null);
				holder = new ViewHolder();
				convertView.setTag(holder);
				holder.groupItem = (TextView) convertView.findViewById(R.id.textView);
				lmap.put(position, convertView);
				 convertView.setTag(holder);
			} else {
				convertView = lmap.get(position);
				holder = (ViewHolder) convertView.getTag();
			}
			holder.groupItem.setText(itemList.get(position));
			return convertView;
		}
		
	}
	private final class ViewHolder {
		TextView groupItem;
	}
}
