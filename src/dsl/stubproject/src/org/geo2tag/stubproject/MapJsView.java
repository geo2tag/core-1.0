package org.geo2tag.stubproject;

import java.util.Locale;


import android.annotation.SuppressLint;
import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Toast;

public class MapJsView extends WebView {

	private int m_mapWidgetInitCounter = 0;
	
	private static final String MAP_FILE = "file:///android_asset/map_tracker.html"; 
	private static final String REFRESH_URL = "javascript:refresh();"; 
	
	private double m_latitude;
	private double m_longitude;
	private boolean m_inited = false;
	
	
	@SuppressLint("SetJavaScriptEnabled")
	public MapJsView(Context context, AttributeSet attrs) {
		super(context, attrs);
						
		WebSettings webSettings = getSettings();
		webSettings.setJavaScriptEnabled(true);
		setWebChromeClient(new WebChromeClient());
		
		setWebViewClient(new WebViewClient() {
		    public void onPageFinished(WebView view, String url) {
		    	if (url.equals(MAP_FILE))
		    		
		    		tryToInitMapWidget();
		    }
		});
		Toast.makeText(getContext(),"Determining current location", Toast.LENGTH_LONG).show();
		
		loadUrl(MAP_FILE);
	}
	
	public synchronized void tryToInitMapWidget(){
		if (m_mapWidgetInitCounter == 1)
			initMapWidget();
		else
			m_mapWidgetInitCounter++;
	}
	
	
	
	public boolean isInited(){
		return m_inited;
	}
	
	
	public void setMapCenter(double latitude, double longitude){
		m_inited = true;
		m_latitude = latitude;
		m_longitude = longitude;
		
		
	}

	private void initMapWidget(){
		Settings settings =  Settings.getInstance();
		
		String login =  settings.getLogin();
		String password =  settings.getPassword();
		String serverUrl =  settings.getServerUrl();
		String dbName =  settings.getDbName();
		int radius = (int) settings.getRadius();
		
		String url = String.format(Locale.US,"javascript:initWithSettings('%s', '%s', %d, '%s', '%s', %.10f, %.10f);", 
				login, password, radius, dbName, serverUrl, m_latitude, m_longitude);
		Log.d(DslUtil.LOG, url);

		loadUrl(url);
	}
	
	public void updateMapWidgetCoordinates(double latitude, double longitude){
		String url = String.format("javascript:updateMapCenter(%f, %f);", 
				latitude, longitude);
		Log.d(DslUtil.LOG, url);
		loadUrl(url);
	}
	
	public void refreshMapWidget(){
		loadUrl(REFRESH_URL);
	}
	

}
