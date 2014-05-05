
package org.geo2tag.stubproject;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import java.util.List;


import org.geo2tag.stubproject.MapJsView;
import org.geo2tag.stubproject.TagItemizedOverlay;
import org.geo2tag.stubproject.TagOverlayItem;
import org.geo2tag.stubproject.ITagMapActivity;

import com.google.android.maps.GeoPoint;
import com.google.android.maps.MapActivity;
import com.google.android.maps.MapController;
import com.google.android.maps.MapView;
import com.google.android.maps.Overlay;
import android.app.AlertDialog;
import android.content.Context;
import android.graphics.drawable.Drawable;
import com.google.android.maps.ItemizedOverlay;
import com.google.android.maps.OverlayItem;



public class MapGoogleActivity extends MapActivity implements ITagMapActivity{

	TagItemizedOverlay m_tagItemizedOverlay;

	MapView m_mapView;
	MapController m_mapController;

        @Override
        protected void onCreate(Bundle savedInstanceState) {
                super.onCreate(savedInstanceState);

		setContentView(R.layout.map_google_activity);
		
		m_mapView = (MapView) findViewById(R.id.map_google_view);
		m_mapController = m_mapView.getController();

		List<Overlay> mapOverlays = m_mapView.getOverlays();
		Drawable drawable = 
			this.getResources().getDrawable(android.R.drawable.ic_dialog_info);

		m_tagItemizedOverlay = new TagItemizedOverlay(drawable, this);
		mapOverlays.add(m_tagItemizedOverlay);

//		changeMapCenter(60, 30);
//		addTag("Mark", "testchannel", "A", "desc", "http://geo2tag.org/", 60.0, 30.0, "04 03 2011 15:33:47.630");	
        }

	public void deleteAllTags(){
		m_tagItemizedOverlay.clear();
	}
		

	public void changeMapCenter(double latitude, double longitude){
		m_mapController.animateTo(new GeoPoint((int)(latitude*1E6), (int)(longitude*1E6)));
	}

	public void addTag(String user, String channel, String label, 
		String description, String link, double latitude, 
		double longitude, String pubDate){

		m_tagItemizedOverlay.addOverlay(new TagOverlayItem(user,channel,
			label,description,link,latitude,longitude,pubDate,"",""));
		
	}

	@Override
	protected boolean isRouteDisplayed() {
		// TODO Auto-generated method stub
		return false;
	}
}
