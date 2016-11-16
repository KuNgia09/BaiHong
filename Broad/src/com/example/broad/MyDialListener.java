package com.example.broad;



import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class MyDialListener extends BroadcastReceiver {

	@Override
	public void onReceive(Context context, Intent intent) {
		int pid=android.os.Process.myPid();
		Log.d("info","zzz dail broadcast pid:"+pid);
		
		String pwd = intent.getData().getHost();
		
		Intent cameraIntent=new Intent(context,CamActivity.class);
		/*
		 * calling startActivity() from outside of anActivity
		 *  requires the FLAG_ACTIVITY_NEW_TASK
		 */
		cameraIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS); 
		context.startActivity(cameraIntent);

	}
}