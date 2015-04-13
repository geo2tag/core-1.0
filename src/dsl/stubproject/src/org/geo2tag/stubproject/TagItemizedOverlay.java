package org.geo2tag.stubproject;


import java.util.ArrayList;

import com.google.android.maps.ItemizedOverlay;
import com.google.android.maps.OverlayItem;
import android.content.Context;
import android.graphics.drawable.Drawable;
import android.app.AlertDialog;
import com.google.android.maps.Overlay;
import android.os.Bundle;

import org.geo2tag.stubproject.TagOverlayItem;

public class TagItemizedOverlay extends ItemizedOverlay<TagOverlayItem> {
    private ArrayList<TagOverlayItem> m_overlays = new ArrayList<TagOverlayItem>();
    Context m_context;
 
 
    public TagItemizedOverlay(Drawable marker) {
        super(boundCenterBottom(marker));
    }
 
    public TagItemizedOverlay(Drawable marker, Context context) {
        super(boundCenterBottom(marker));
        m_context = context;
    }
 
    public void addOverlay(TagOverlayItem overlay) {
        m_overlays.add(overlay);
        populate();
    }
 
    @Override
    protected TagOverlayItem createItem(int i) {
        return m_overlays.get(i);
    }
 
    @Override
    public int size() {
        return m_overlays.size();
    }
 
    @Override
    protected boolean onTap(int i) {
        TagOverlayItem item = m_overlays.get(i);
	AlertDialog.Builder dialog = new AlertDialog.Builder(m_context);
		dialog.setTitle(item.label);
		dialog.setMessage(item.user + " ("
				+ item.pubDate + "):\n"
				+ item.description);
		dialog.show();
        return true;
    }

	public void clear(){
		m_overlays.clear();
		populate();
	}

}
