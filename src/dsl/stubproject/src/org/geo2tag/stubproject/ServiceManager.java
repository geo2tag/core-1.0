package org.geo2tag.stubproject;

import android.app.ActivityManager;
import android.app.Service;
import android.app.ActivityManager.RunningServiceInfo;
import android.content.Context;
import android.content.Intent;



public class ServiceManager{


    @SuppressWarnings("static-access")
	public static boolean isServiceRunning(Context c, Class<? extends Service> serviceClass){
        ActivityManager manager = (ActivityManager) c.getSystemService(c.ACTIVITY_SERVICE);
        for (RunningServiceInfo service : manager.getRunningServices(Integer.MAX_VALUE)) {
            if (serviceClass.getName().equals(service.service.getClassName())) {
                return true;
            }
        }
        return false;
    }
	
	public static void startService(Context context, Class<? extends Service> serviceClass){
		context.startService(new Intent(context, serviceClass));
	}
	
	public static void stopService(Context context, Class<? extends Service> serviceClass){
		context.stopService(new Intent(context, serviceClass));
	}
}
