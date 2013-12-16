package org.geo2tag.stubproject;

public interface ITagMapActivity {

	public void deleteAllTags();
		

	public void changeMapCenter(double latitude, double longitude);

	public void addTag(String user, String channel, String label, 
		String description, String link, double latitude, 
		double longitude, String pubDate);
	
}
