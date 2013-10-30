/*
 * Copyright 2010-2012  Vasily Romanikhin  bac1ca89@gmail.com
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */
package com.example.stubproject;


import java.util.Date;


import ru.spb.osll.Geo2TagRequests;
import ru.spb.osll.json.*;
import android.app.ActivityManager;
import android.app.Service;
import android.app.ActivityManager.RunningServiceInfo;
import android.content.Context;
import android.content.Intent;
import android.location.Location;
import android.os.IBinder;
import android.util.Log;

public class TrackableService extends LocationService {

	private static boolean s_online = true;


	public TrackableService(String channelName, String broadcastActionName,
			int updateInterval) {
		super(channelName, broadcastActionName, updateInterval);
	}

	private static final String TAG_TITLE = "";
	private static final String TAG_DESCRIPTION = "";
	private static final String TAG_LINK = "";
	private static final int MAX_TRACK_FAILS = 3;


	@Override
	public void onStart(Intent intent, int startId) {
		Log.v(DslUtil.LOG, "TrackableService.onStart()");
		super.onStart(intent, startId);
	}

	@Override
	public void onDestroy() {
		Log.v(DslUtil.LOG, "TrackableService.onDestroy()");
		super.onDestroy();
	}

	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}

	public boolean addTag(){
		if (isDeviceReady()) {
			try{
				final Location location = getLocation(this);

				if (location == null) return false;

				final Date date = new Date();

				Geo2TagRequests.applyMark(getAuthToken(), TAG_TITLE, TAG_DESCRIPTION,
						TAG_LINK, location.getLatitude(), location.getLongitude(),
						location.getAltitude(), getChannelName(), date, getSettings().getServerUrl());

				return true;
			}catch (RequestException e){

				return false;
			}
		}

		return false;

	}
	
	@Override
	protected void periodicAction() throws RequestException{
		track();
	}

	public void track() throws RequestException {
		int failCount = 0;
		while (isServiceUp()){
			try {
				if (!addTag()) 
					failCount++;

				if (failCount > MAX_TRACK_FAILS) {
					throw new RequestException(Errno.TRACK_ATTEMPTS_FAILED_ERROR);
				}
				Thread.sleep(getUpdateInterval()*1000);
			} catch (InterruptedException e) {
				setIsServiceUp(false);
			}
		}
	}
	
	private static Class<?> getClassStaticaly(){
		Class<?> result = new Object() {}.getClass().getEnclosingClass();
		
		return result;
	}
	
	
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
	
	public static void startTracking(Context context){
		context.startService(new Intent(context, getClassStaticaly()));
	}
	
	public static void stopTracking(Context context){
		context.stopService(new Intent(context, getClassStaticaly()));
	}

	public static void setOnline(boolean online){
		s_online = online;
	}

}
