
package org.geo2tag.stubproject;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import org.geo2tag.stubproject.MapJsView;
import org.geo2tag.stubproject.ITagMapActivity;


public class MapJsActivity extends Activity implements ITagMapActivity{

	private MapJsView m_mapJsView ;


        @Override
        protected void onCreate(Bundle savedInstanceState) {
                super.onCreate(savedInstanceState);

		setContentView(R.layout.map_osm_activity);

		m_mapJsView = (MapJsView)findViewById(R.id.map_js_view);
//		m_mapJsView.changeMapCenter(60, 30);
//		m_mapJsView.addTag("Mark", "testchannel", "A", "desc", "http://geo2tag.org/", 60.0, 30.0, "04 03 2011 15:33:47.630");	
        }


	public void deleteAllTags(){
		m_mapJsView.deleteAllTags();
	}
		

	public void changeMapCenter(double latitude, double longitude){
		m_mapJsView.changeMapCenter(latitude, longitude);
	}

	public void addTag(String user, String channel, String label, 
		String description, String link, double latitude, 
		double longitude, String pubDate){

		m_mapJsView.addTag(user, channel, label, description, link, latitude, longitude, pubDate);	
	}

}
