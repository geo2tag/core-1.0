package org.geo2tag.stubproject;

public class NAME_PLACEHOLDER extends MonitorService {


	@Override
	public void onCreate() {
		setBroadcastActionName("action.NAME_PLACEHOLDER");
		setUpdateInterval(100);
		setMonitorCondition(new MonitorCondition("MONITOR_CONDITION_PLACEHOLDER"));
	}
}
