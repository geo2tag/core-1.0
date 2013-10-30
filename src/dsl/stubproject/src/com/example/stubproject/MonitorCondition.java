package com.example.stubproject;

import java.util.Map;

import ru.spb.osll.objects.Channel;

public class MonitorCondition {

	private String m_condition;
	
	public MonitorCondition(String condition){
		m_condition = condition;
	}
	
	/**
	 *  
	 * @param channelsState - map <ChannelName, IsChanged>
	 * @return true if the given list of the channel states match conditions
	 */
	public boolean checkCondition(Map<Channel, Boolean> channelsState){
		// TODO add more complex logic for checking condition
		return true;
		
	}
}
