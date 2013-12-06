
package org.geo2tag.stubproject;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import org.geo2tag.stubproject.MapJsView;

public class MapActivity extends Activity {

	private MapJsView m_mapJsView ;


        @Override
        protected void onCreate(Bundle savedInstanceState) {
                super.onCreate(savedInstanceState);

		setContentView(R.layout.map_osm_activity);

		m_mapJsView = (MapJsView)findViewById(R.id.map_js_view);
		m_mapJsView.changeMapCenter(60, 30);
		m_mapJsView.addTag("Mark", "testchannel", "A", "desc", "http://geo2tag.org/", 60.0, 30.0, "04 03 2011 15:33:47.630");	
        }


}
