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
 * PROJ: OSLL/geo2tag
 */

//package org.geo2tag.tracker.services;

package org.geo2tag.stubproject;


import java.util.List;


import ru.spb.osll.Geo2TagRequests;
import ru.spb.osll.json.RequestException;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;

public class LocationService extends Service {

	public static final String ACTION_LOCATION  = "action.location";
	public static final String TYPE_LOCATION_TEXT = "type.location_text";
	public static final String TYPE_LOCATION_OBJ = "type.loaction_obj";

	public static final String TYPE_MESSAGE = "message";
	public static final String TYPE_OPEATION = "operation";
	
	public static final String OPERATION_ERROR = "operation.error";
	public static final String OPERATION_DATA = "operation.data";

	private String m_channelName = null;
	private String m_broadcastActionName = null;
	private int m_updateInterval;
	private boolean m_online = true;
	
	private Settings m_settings = Settings.getInstance();

	
	private String m_authToken = null;

	private boolean m_isServiceUp = false;

	private static boolean isDeviceReady = false;


	@Override
	public void onStart(Intent intent, int startId) {
		super.onStart(intent, startId);

		if (isServiceUp())
			return;
		
		setIsServiceUp(true);	

		isDeviceReady = false;

		Log.d(DslUtil.LOG, "LocationService.onStart() "+isDeviceReady());

		LocationManager locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
		locationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER, 0, 0, mLocationListener);
		locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0, mLocationListener);  
		
		new Thread(new Runnable() {
			@Override
			public void run() {
				try {

					prepareChannel();

					periodicAction();
				} catch (RequestException e) {

					sendErrorMessage(e.getMessage());
					stopSelf();
				}
			}
		}).start();
		
	}

	protected  void periodicAction() throws RequestException{
		/// STUB, redefine it
		throw new UnsupportedOperationException("Must implement!!!!");
	}
	
	@Override
	public void onDestroy() {
		Log.d(DslUtil.LOG, "LocationService.onDestroy()");
		setIsServiceUp(false);
		LocationManager locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
		locationManager.removeUpdates(mLocationListener);
	}

	private LocationListener mLocationListener = new LocationListener() {
		public void onLocationChanged(Location location) {
			if (location != null)
				sendLocation(location);

			if (!isDeviceReady && location != null) {
				isDeviceReady = true;
			} else if (isDeviceReady && location == null) {
				isDeviceReady = false;
			}
		}

		public void onStatusChanged(String provider, int status, Bundle extras) {
			Log.d(DslUtil.LOG, "onStatusChanged: " + provider + ", " + status);
		}

		public void onProviderEnabled(String provider) {
			Log.d(DslUtil.LOG, "onProviderEnabled: " + provider);
		}

		public void onProviderDisabled(String provider) {
			Log.d(DslUtil.LOG, "onProviderDisabled: " + provider);
		}
	};

	public static boolean isDeviceReady(){
		return isDeviceReady;
	}

	public static Location getLocation(Context context) {
		Location bestResult = null;
		long minTime = 30 * 1000;   // 1 sec
		float bestAccuracy = Float.MAX_VALUE;
		long bestTime = Long.MIN_VALUE;

		LocationManager  locationManager = (LocationManager) context.getSystemService(Context.LOCATION_SERVICE);
		List<String> matchingProviders = locationManager.getAllProviders();
		for (String provider : matchingProviders) {
			Location location = locationManager.getLastKnownLocation(provider);
			if (location != null) {
				float accuracy = location.getAccuracy();
				long time = location.getTime();
				if ((time > minTime && accuracy < bestAccuracy)) {
					bestResult = location;
					bestAccuracy = accuracy;
					bestTime = time;
				} else if (time < minTime && bestAccuracy == Float.MAX_VALUE
						&& time > bestTime) {
					bestResult = location;
					bestTime = time;
				}
			}
		}



		return bestResult;
	}	

	private  void sendLocation(Location location) {
		Intent intent = new Intent(ACTION_LOCATION);
		intent.putExtra(TYPE_LOCATION_TEXT, 
				DslUtil.convertLocation(location.getLatitude(), location.getLongitude()));
		intent.putExtra(TYPE_LOCATION_OBJ, location);

		sendBroadcast(intent);
	}


	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}   
	

	protected void sendErrorMessage(String message) {
		sendMessage(getBroadcastActionName(),
				OPERATION_ERROR, message);
	}
	
	protected void sendDataMessage(String message) {
		sendMessage(getBroadcastActionName(),
				OPERATION_DATA, message);
	}

	protected void sendMessage(String action, String operation, String message) {
		Intent intent = new Intent(action);
		intent.putExtra(TYPE_OPEATION, operation.toString());
		intent.putExtra(TYPE_MESSAGE, message);
		sendBroadcast(intent);
	}

	public String getChannelName() {
		return m_channelName;
	}

	public void setChannelName(String m_channelName) {
		this.m_channelName = m_channelName;
	}

	public String getBroadcastActionName() {
		return m_broadcastActionName;
	}

	public void setBroadcastActionName(String m_broadcastActionName) {
		this.m_broadcastActionName = m_broadcastActionName;
	}

	public int getUpdateInterval() {
		return m_updateInterval;
	}

	public void setUpdateInterval(int m_updateInterval) {
		this.m_updateInterval = m_updateInterval;
	}

	public boolean isServiceUp() {
		return m_isServiceUp;
	}

	public void setIsServiceUp(boolean m_isServiceUp) {
		this.m_isServiceUp = m_isServiceUp;
	}

	public String getAuthToken() {
		return m_authToken;
	}

	public void setAuthToken(String m_authToken) {
		this.m_authToken = m_authToken;
	}
	
	protected void prepareChannel() throws RequestException{
		m_authToken = Geo2TagRequests.login(getSettings().getLogin(),
				getSettings().getPassword(), 
				getSettings().getServerUrl());

		Geo2TagRequests.setDb(m_authToken, getSettings().getDbName(),
				getSettings().getServerUrl());

		Geo2TagRequests.applyChannel(m_authToken, getChannelName(),
				"", "", getSettings().getServerUrl());

		Geo2TagRequests.subscribeToChannel(m_authToken, getChannelName(), 
				getSettings().getServerUrl());
	}

	public Settings getSettings() {
		return m_settings;
	}

	public void setSettings(Settings m_settings) {
		this.m_settings = m_settings;
	}



	public boolean isOnline(){
		return m_online;
	}

	public void setOnline(boolean online){
		m_online = online;
	}
	
}
