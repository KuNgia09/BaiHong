package com.example.numbercamera;

public class ImageName {
	
	private int length = 15;
	
	public String getImageName(){
		return RamdomName();
	}
	
	public String getFileName(){
		return RamdomName();
	}
	
	private String RamdomName(){
		
		String name = "";
		String chars = "123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		for (int i = 0; i < length; i++) {
			name = name + chars.charAt((int)(Math.random() * (chars.length())));
		}
		return name;
	}
}
