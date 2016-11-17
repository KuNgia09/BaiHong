package com.example.utils;

import java.io.File;
import java.util.ArrayList;

public abstract class CheckFileName {

	//检查随机的名字是否重复
	public static boolean checkRandomName(String name,ArrayList<String> filesname){
		
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
	
	//枚举加密目录所有的文件名
	public static ArrayList<String> getFiles(String path){
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
