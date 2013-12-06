package org.geo2tag.stubproject;

import java.util.Locale;
import java.util.List;
import java.util.ArrayList;


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

	
	private static final String MAP_FILE = "file:///android_asset/map.html"; 


	private static final String ADD_TAG_URL = "javascript:mapWidget.addTag('%s', '%s', '%s', '%s', '%s', %.10f, %.10f, '%s');"; 
	private static final String DELETE_ALL_TAGS_URL = "javascript:mapWidget.removeAllTagsFromMap();"; 
	private static final String CHANGE_MAP_CENTER_URL = "javascript:mapWidget.changeMapCenter(%.10f, %.10f);"; 
	
	private List<String> m_urlList = new ArrayList<String>();
	private boolean m_mapIsLoading = false;
	
	
	@SuppressLint("SetJavaScriptEnabled")
	public MapJsView(Context context, AttributeSet attrs) {
		super(context, attrs);
						
		WebSettings webSettings = getSettings();
		webSettings.setJavaScriptEnabled(true);
		setWebChromeClient(new WebChromeClient());
		
		setWebViewClient(new WebViewClient() {
		    public void onPageFinished(WebView view, String url) {
			Log.d("DSL", "Finished "+url+" loading");
			m_mapIsLoading = false;
			
			for (String s: m_urlList){
				Log.d("DSL", "Start loading: "+s);
				loadUrlSync(s);
			}
			//m_urlList.clear();
		    }
		});
		
		// Create lock
		loadUrlSync(MAP_FILE);
	}
	
	
	
	private synchronized void loadUrlSync(String url)
	{
		Log.d("DSL", url+" m_urlIsLoading = "+m_mapIsLoading);
		if (m_mapIsLoading ){
			Log.d("DSL", "Putting url to wait list: "+url);
			m_urlList.add(url);
		}else{
			Log.d("DSL", "Loading url immideately: "+url);
			if (url.equals(MAP_FILE))
				m_mapIsLoading = true;
			loadUrl(url);
		}
	}	
	
	
	public void deleteAllTags(){
		loadUrlSync(DELETE_ALL_TAGS_URL);
	}

	public void changeMapCenter(double latitude, double longitude){
		String url = String.format(Locale.US, CHANGE_MAP_CENTER_URL,
			latitude, longitude);
		loadUrlSync(url);	
		
	}

	public void addTag(String user, String channel, String label, 
		String description, String link, double latitude, 
		double longitude, String pubDate){

		String url = String.format(Locale.US, ADD_TAG_URL,
		user,channel,label, description, link, latitude, longitude, pubDate);
		
		loadUrlSync(url);
	} 

}
