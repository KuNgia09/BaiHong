package com.example.numbercamera;

import java.io.File;
import java.util.ArrayList;

public class Check {
	public boolean CheckName(String name,ArrayList<String> filesname){
		//检查随机的名字是否重复了
		if (filesname == null) {
			return true;
		}
		 for(int i=0; i<filesname.size(); i++){   
			 if (name.equals(filesname.get(i))) {
				return false;
			 }
		  }   
		return true;
	}
	
	public ArrayList<String> getFiles(String path){
		File file = new File(path);
		if (file.listFiles() == null) {
			return null;
		}
		//获取存储目录下的
		File [] files = file.listFiles();
		
		ArrayList<String> filesname = new ArrayList<String>();
		for (int i = 0; i < files.length; i++) {
			filesname.add(files[i].getName());
		}
		return filesname;
	}	
}
