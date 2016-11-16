package com.example.numbercamera;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class HomeWatcherReceiver extends BroadcastReceiver{
	
	private static final String LOG_TAG = "HomeReceiver";
	private static final String SYSTEM_DIALOG_REASON_KEY = "reason";
	private static final String SYSTEM_DIALOG_REASON_RECENT_APPS = "recentapps";
	private static final String SYSTEM_DIALOG_REASON_HOME_KEY = "homekey";
	private static final String SYSTEM_DIALOG_REASON_LOCK = "lock";
	private static final String SYSTEM_DIALOG_REASON_ASSIST = "assist";
	private Activity activity;
	
	public HomeWatcherReceiver(Activity activity) {
		this.activity = activity;
	}
	    
	@Override
	public void onReceive(Context context, Intent intent) {
		String action = intent.getAction();
		Log.i(LOG_TAG, "onReceive: action: " + action);
		if (action.equals(Intent.ACTION_CLOSE_SYSTEM_DIALOGS)) {
			// android.intent.action.CLOSE_SYSTEM_DIALOGS
			String reason = intent.getStringExtra(SYSTEM_DIALOG_REASON_KEY);
			Log.i(LOG_TAG, "reason: " + reason);
			activity.finish();
			if (SYSTEM_DIALOG_REASON_HOME_KEY.equals(reason)) {
				// �̰�Home��
				Log.i(LOG_TAG, "homekey");

			} else if (SYSTEM_DIALOG_REASON_RECENT_APPS.equals(reason)) {
				// ����Home�� ���� activity�л���
				Log.i(LOG_TAG, "long press home key or activity switch");

			} else if (SYSTEM_DIALOG_REASON_LOCK.equals(reason)) {
				// ����
				Log.i(LOG_TAG, "lock");
			} else if (SYSTEM_DIALOG_REASON_ASSIST.equals(reason)) {
				// samsung ����Home��
				Log.i(LOG_TAG, "assist");
			}

		}
	}
	

}
