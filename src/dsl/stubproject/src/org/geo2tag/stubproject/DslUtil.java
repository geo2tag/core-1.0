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

/*! ---------------------------------------------------------------
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

package org.geo2tag.stubproject;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;


import android.app.ActivityManager;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.ActivityManager.RunningServiceInfo;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.location.Location;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;

public class DslUtil {
    
	private static CharSequence m_log = "";
	
	public static final String LOG = "Geo2Tag-DSL";

	private static DateFormat dateFormat = new SimpleDateFormat("dd MM yyyy HH:mm:ss.SSS");
	public static String getTime(Date date){
		return dateFormat.format(date);
	}
	
	private static DateFormat dateFormatForLog = new SimpleDateFormat("HH:mm:ss");
	public static String getTimeForLog(Date date){
		return dateFormatForLog.format(date);
	}
	
	
	public static String convertLocation(double latitude, double longitude){
		StringBuffer strBuffer = new StringBuffer();
		strBuffer.append("lat: ").append(Location.convert(latitude, Location.FORMAT_MINUTES)).
		append("  lon: ").append(Location.convert(longitude, Location.FORMAT_MINUTES));	
		return strBuffer.toString();
	}
	

	
	public static boolean isOnline(Context c) {
	    ConnectivityManager cm = (ConnectivityManager)c.getSystemService(Context.CONNECTIVITY_SERVICE);
	    NetworkInfo nInfo = cm.getActiveNetworkInfo();
	    if (nInfo != null && nInfo.isConnected()) {
	        return true;
	    }
	    else {
	        return false;
	    }
	}
	
	@SuppressWarnings("static-access")
	public static boolean isServiceRunning(Context c, Class<? extends Service> className){
		ActivityManager manager = (ActivityManager) c.getSystemService(c.ACTIVITY_SERVICE);
		for (RunningServiceInfo service : manager.getRunningServices(Integer.MAX_VALUE)) {
    			if (className.getName().equals(service.service.getClassName())) {
				return true;
			}
		}
		return false;
	}
	
	public static CharSequence getLogText() {
		return m_log;
	}

	public static void setLogText(CharSequence m_log) {
		DslUtil.m_log = m_log;
	}

	public class Logger {
		private PrintWriter m_logger;
		
		public Logger(String logPath){
			try {
				m_logger = new PrintWriter(new OutputStreamWriter(new FileOutputStream(logPath, true), "UTF-8"));
				m_logger.println(dateFormat.format(new Date()) + " \t -------------- START LOG -------------");
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
		public void println(String str){
			m_logger.println(dateFormat.format(new Date()) + "\t\t:" + str);
		}

		public void destroy(){
			if (m_logger != null)
				m_logger.close();
		}
	}
}
