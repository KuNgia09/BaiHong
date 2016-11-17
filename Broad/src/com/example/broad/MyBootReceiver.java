package com.example.broad;

import com.example.utils.DLog;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class MyBootReceiver extends BroadcastReceiver {

	@Override
	public void onReceive(Context context, Intent intent) {
		// TODO Auto-generated method stub
		int pid=android.os.Process.myPid();
		DLog.d("info","hacked by jack pid:"+pid);

	}

}
