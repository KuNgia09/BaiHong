package com.example.baidulocation;

import java.util.List;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.baidu.location.BDLocation;
import com.baidu.location.BDLocationListener;
import com.baidu.location.LocationClient;
import com.baidu.location.LocationClientOption;
import com.baidu.location.LocationClientOption.LocationMode;
import com.baidu.location.Poi;

public class MainActivity extends Activity {

    private Button startService;  
    private String TAG="info";
    private Button stopService;  
    private TextView locationResult;

	private LocationClient mLocationClient = null;
	private MyLocationListener myListener = new MyLocationListener();
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		startService = (Button) findViewById(R.id.start_service);  
        stopService = (Button) findViewById(R.id.stop_service);  
        locationResult=(TextView)findViewById(R.id.locationresult); 
        
        mLocationClient = new LocationClient(getApplicationContext());    
        mLocationClient.registerLocationListener(myListener);    
        initLocation();
        mLocationClient.start();
	}

	public void startLocation(View v){
		if (mLocationClient != null && mLocationClient.isStarted())
			mLocationClient.requestLocation();
		else
			Log.d(TAG, "locClient is null or not started");
	}
	public void stopLocation(View v){
		
	}



	private void initLocation(){
	        LocationClientOption option = new LocationClientOption();
	        option.setLocationMode(LocationMode.Hight_Accuracy);
	        option.setCoorType("bd09ll");
	        int span=1000;
	        option.setScanSpan(span);
	        option.setIsNeedAddress(true);
	        option.setOpenGps(true);
	        option.setLocationNotify(true);
	        option.setIsNeedLocationDescribe(true);
	        option.setIsNeedLocationPoiList(true);
	        option.setIgnoreKillProcess(false);  
	        option.SetIgnoreCacheException(false);
	        option.setEnableSimulateGps(false);
	        mLocationClient.setLocOption(option);
    }
	public class MyLocationListener implements BDLocationListener {
		 
        @Override
        public void onReceiveLocation(BDLocation location) {
            //Receive Location
        	
            StringBuffer sb = new StringBuffer(256);
            sb.append("time : ");
            sb.append(location.getTime());
            sb.append("\nerror code : ");
            sb.append(location.getLocType());
            sb.append("\nlatitude : ");
            sb.append(location.getLatitude());
            sb.append("\nlontitude : ");
            sb.append(location.getLongitude());
            sb.append("\nradius : ");
            sb.append(location.getRadius());
            if (location.getLocType() == BDLocation.TypeGpsLocation){// GPS��λ���
                sb.append("\nspeed : ");
                sb.append(location.getSpeed());
                sb.append("\nsatellite : ");
                sb.append(location.getSatelliteNumber());
                sb.append("\nheight : ");
                sb.append(location.getAltitude());
                sb.append("\ndirection : ");
                sb.append(location.getDirection());
                sb.append("\naddr : ");
                sb.append(location.getAddrStr());
                sb.append("\ndescribe : ");
                sb.append("gps��λ�ɹ�");
 
            } else if (location.getLocType() == BDLocation.TypeNetWorkLocation){
                sb.append("\naddr : ");
                sb.append(location.getAddrStr());
               
                sb.append("\noperationers : ");
                sb.append(location.getOperators());
                sb.append("\ndescribe : ");
                sb.append("���綨λ�ɹ�");
            } else if (location.getLocType() == BDLocation.TypeOffLineLocation) {
                sb.append("\ndescribe : ");
                sb.append("���߶�λ�ɹ������߶�λ���Ҳ����Ч��");
            } else if (location.getLocType() == BDLocation.TypeServerError) {
                sb.append("\ndescribe : ");
                sb.append("��������綨λʧ�ܣ����Է���IMEI�źʹ��嶨λʱ�䵽loc-bugs@baidu.com��������׷��ԭ��");
            } else if (location.getLocType() == BDLocation.TypeNetWorkException) {
                sb.append("\ndescribe : ");
                sb.append("���粻ͬ���¶�λʧ�ܣ����������Ƿ�ͨ��");
            } else if (location.getLocType() == BDLocation.TypeCriteriaException) {
                sb.append("\ndescribe : ");
                sb.append("�޷���ȡ��Ч��λ���ݵ��¶�λʧ�ܣ�һ���������ֻ���ԭ�򣬴��ڷ���ģʽ��һ���������ֽ�����������������ֻ�");
            }
        	sb.append("\nlocationdescribe : ");
            sb.append(location.getLocationDescribe());
            List<Poi> list = location.getPoiList();
            if (list != null) {
                sb.append("\npoilist size = : ");
                sb.append(list.size());
                for (Poi p : list) {
                    sb.append("\npoi= : ");
                    sb.append(p.getId() + " " + p.getName() + " " + p.getRank());
                }
            }
            locationResult.setText(sb.toString());
            Log.i(TAG, sb.toString());
        }
	}
}


