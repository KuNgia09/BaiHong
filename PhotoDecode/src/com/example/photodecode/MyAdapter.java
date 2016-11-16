package com.example.photodecode;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.CheckBox;
import android.widget.ImageView;
import android.widget.TextView;

public class MyAdapter extends BaseAdapter{
	
	private ArrayList<Map<String,Object>> list;
	private static HashMap<Integer, Boolean> isSelected ;
	private Context context;
	private LayoutInflater inflater = null;
	
	public MyAdapter(ArrayList<Map<String,Object>> list , Context context) {
		this.context = context;
		this.list = list;
		isSelected = new HashMap<Integer, Boolean>();
		inflater = LayoutInflater.from(context);
		initDate();
		System.out.println();
	}
	
	private void initDate(){
		for (int i = 0; i < list.size(); i++) {
			getIsSeleted().put(i, false);
		}
	}
	
	public static void setIsSelected(HashMap<Integer, Boolean> isSelected) {
		MyAdapter.isSelected = isSelected;
	}
	
	public static HashMap<Integer, Boolean> getIsSeleted(){
		return isSelected;
	}
	
	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return list.size();
	}

	@Override
	public Object getItem(int position) {
		// TODO Auto-generated method stub
		return list.get(position);
	}

	@Override
	public long getItemId(int position) {
		// TODO Auto-generated method stub
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		ViewHolder holder = null;
		if (convertView == null) {
			holder = new ViewHolder();
			convertView = inflater.inflate(R.layout.activity_select, null);
			holder.name = (TextView) convertView.findViewById(R.id.name);
			holder.path = (TextView) convertView.findViewById(R.id.path);
			holder.img = (ImageView) convertView.findViewById(R.id.img);
			holder.checkBox = (CheckBox) convertView.findViewById(R.id.checkbox);
			
			convertView.setTag(holder);
			
		}else {
			holder = (ViewHolder) convertView.getTag();
		}
		
		holder.name.setText(list.get(position).get("name").toString());
		holder.path.setText(list.get(position).get("path").toString());
		if ( !(list.get(position).get("img") == null)) {
			holder.img.setImageResource((Integer) list.get(position).get("img"));
		}
		
		File file = new File(list.get(position).get("path").toString());
		
		if (file.isDirectory()) {
			holder.checkBox.setVisibility(View.INVISIBLE);
		}else{
			holder.checkBox.setVisibility(View.VISIBLE);
			holder.checkBox.setChecked(getIsSeleted().get(position));
		}
		return convertView;
	}
	
	
	public final class ViewHolder{
		public TextView name;
		public TextView path;
		public CheckBox checkBox;
		public ImageView img;
	}
}
