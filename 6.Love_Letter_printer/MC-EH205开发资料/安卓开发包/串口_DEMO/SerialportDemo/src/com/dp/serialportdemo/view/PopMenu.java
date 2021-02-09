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
 * 修改于：2015-6-17
 * 
 *  修正 ListView item 点击响应失败！
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

	// 设置菜单项点击监听器
	public void setOnItemClickListener(OnItemClickListener listener) {
		 this.listener = listener;
	}
	
	// 批量添加菜单项
	public void addItems(String[] items) {
		for (String s : items)
			itemList.add(s);
	}
	// 批量添加菜单项
	public void clearItems() {		
		itemList.clear();
	}

	// 单个添加菜单项
	public void addItem(String item) {
		itemList.add(item);
	}
	public String getItem(int index)
	{
		return itemList.get(index);
	}

	// 下拉式 弹出 pop菜单 parent 右下角
	public void showAsDropDown(View parent,Context context,int ww) {
		inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		View view = inflater.inflate(R.layout.popmenu, null);
		listView = (ListView) view.findViewById(R.id.listView);
		Log.e("================PopMenu=="+parent.getWidth(), "===================="+ww);
		listView.getLayoutParams().width=ww;
		listView.setAdapter(pa);
		listView.setOnItemClickListener(this);		
		popupWindow = new PopupWindow(view,LayoutParams.WRAP_CONTENT,  //这里宽度需要自己指定，使用 WRAP_CONTENT 会很大
				LayoutParams.WRAP_CONTENT);
//		popupWindow.showAtLocation(parent, Gravity.RIGHT, 30, -20);// 居中来显示
		// 这个是为了点击“返回Back”也能使其消失，并且并不会影响你的背景
		popupWindow.setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
		popupWindow.showAsDropDown(parent);
		// 使其聚集
		popupWindow.setFocusable(true);
		// 设置允许在外点击消失
		popupWindow.setOutsideTouchable(true);
		// 刷新状态
		popupWindow.update();
		
	}

	// 隐藏菜单
	public void dismiss() {
		if(popupWindow!=null)
		{
			popupWindow.dismiss();	
		}
		
	}

	// 适配器
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
