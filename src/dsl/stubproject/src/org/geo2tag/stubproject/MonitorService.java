package org.geo2tag.stubproject;


import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import ru.spb.osll.Geo2TagRequests;
import ru.spb.osll.json.RequestException;
import ru.spb.osll.objects.Channel;
import ru.spb.osll.utils.DateUtil;
import android.content.Intent;
import android.location.Location;
import android.os.IBinder;

public class MonitorService extends LocationService {

	
	/////////////////////// This fields will be defined by user service description
	private MonitorCondition m_condition;
	
	///////////////////////
	
	private List<Channel> m_oldChannels = null;
	
	
	public MonitorService(String channelName, String broadcastActionName, MonitorCondition condition, int refreshInterval ){
		super(channelName, broadcastActionName, refreshInterval);
		
		m_condition = condition;
	
	}

	@Override
	protected void periodicAction() throws RequestException{
		checkCondition();
	}
	
	private void checkCondition() throws RequestException{
		while (isServiceUp()){
			try {
				// TODO add channel changes collecting routines
				//Geo2TagRequests.filterCircle
				Map<Channel, Boolean> channelsState = checkChannelsState();
				
				if (m_condition.checkCondition(channelsState)){
					// Generate event 
					sendMessage(getBroadcastActionName(), OPERATION_DATA, "TRIGGERED!!!");
				}
				
				Thread.sleep(getUpdateInterval()*1000);
			} catch (InterruptedException e) {
				setIsServiceUp(false);
			}
		}
		
	}
	
	private Map<Channel, Boolean> checkChannelsState() throws RequestException {
		Map<Channel, Boolean> result = null;
		Location location = getLocation(this);
		if (location != null){
			String timeFrom = DateUtil.getDateWithShiftString(getSettings().getDayShift());
			String timeTo = DateUtil.getDateString(new Date());
			List<Channel> channels = Geo2TagRequests.filterCircle(getAuthToken(), location.getLatitude(),
					location.getLongitude(), getSettings().getRadius(), timeFrom, timeTo,
					getSettings().getServerUrl());
			
			result = generateChangesMap(channels);
			
		}
				
				
		return result;
	}



	private Map<Channel, Boolean> generateChangesMap(List<Channel> newChannels){
		Map<Channel, Boolean> result  = new HashMap<Channel, Boolean>();

		/// Add actual list comparison
		
		m_oldChannels = newChannels;
		return result;
	}

	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}
	
}
