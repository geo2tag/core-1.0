package org.geo2tag.stubproject;

public class MONITOR_NAME_PLACEHOLDER extends MonitorService {


	@Override
	public void onCreate() {
		setBroadcastActionName("BROADCAST_ACTION_PLACEHOLDER");
		setUpdateInterval(UPDATE_INTERVAL_PLACEHOLDER);
		setMonitorCondition(new MonitorCondition("MONITOR_CONDITION_PLACEHOLDER"));
	}
}
