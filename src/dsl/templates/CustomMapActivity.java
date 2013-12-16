

package org.geo2tag.stubproject;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import org.geo2tag.stubproject.BASE_ACTIVITY_PLACEHOLDER;

public class CustomMapActivity extends BASE_ACTIVITY_PLACEHOLDER{


        @Override
        protected void onCreate(Bundle savedInstanceState) {
                super.onCreate(savedInstanceState);

		changeMapCenter(60, 30);
		addTag("Mark", "testchannel", "A", "desc", "http://geo2tag.org/", 60.0, 30.0, "04 03 2011 15:33:47.630");	
		addTag("Mark", "testchannel", "B", "desc", "http://geo2tag.org/", 50.0, 20.0, "04 03 2011 15:33:47.630");	
		addTag("Mark", "testchannel", "A", "desc", "http://geo2tag.org/", 45.0, 35.0, "04 03 2011 15:33:47.630");	
        }
}
