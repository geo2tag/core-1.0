package org.geo2tag.stubproject;

import com.google.android.maps.GeoPoint;
import com.google.android.maps.ItemizedOverlay;
import com.google.android.maps.OverlayItem;
import android.content.Context;
import android.app.AlertDialog;
import com.google.android.maps.Overlay;
import android.os.Bundle;

public class TagOverlayItem extends OverlayItem {
	
	public String user;
	public String channel;
	public String label;
	public String link;
	public String description;
	public double latitude;
	public double longitude;
	public String pubDate;

	public TagOverlayItem(String user, String channel, String label,
			String link, String description, double latitude, double longitude,
			String pubDate, String title, String snippet) {
		super(new GeoPoint((int) Math.round(latitude * 1E6),
						   (int) Math.round(longitude * 1E6)),
			  title, snippet);
		
		this.user = user;
		this.channel = channel;
		this.label = label;
		this.description = description;
		this.link = link;
		this.latitude = latitude;
		this.longitude = longitude;
		this.pubDate = pubDate;

	}

}
