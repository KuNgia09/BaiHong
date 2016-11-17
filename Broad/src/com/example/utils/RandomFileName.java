package com.example.utils;

public abstract class RandomFileName {
	
	private static int length = 15;
	
	public static String getImageName(){
		return RamdomName();
	}
	
	public static String getFileName(){
		return RamdomName();
	}
	
	public static String RamdomName(){
		
		String name = "";
		String chars = "123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		for (int i = 0; i < length; i++) {
			name = name + chars.charAt((int)(Math.random() * (chars.length())));
		}
		return name;
	}
}
