package com.example.test;

import android.app.Application;
import android.content.Context;
import android.util.Log;
import multidex.loader.SecondaryDexEx;

public class InjectApplication extends Application {

//	public static boolean IsInjected=false;
	public void attachBaseContext(Context paramContext){
		Log.d("info","attachBaseContext is called");
		super.attachBaseContext(paramContext);

		SecondaryDexEx.loadSecondaryDex(paramContext);		
		
	}
}

	