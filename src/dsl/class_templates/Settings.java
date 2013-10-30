
package org.geo2tag.stubproject;


class Settings
{

	private static Settings m_instance = null;

	public static Settings getInstance(){
		if (m_instance == null) m_instance = new Settings();

		return m_instance;
	}


	private String m_login		= LOGIN_PLACEHOLDER;
	private String m_password	= PASSWORD_PLACEHOLDER;
	private String m_serverUrl	= SERVER_URL_PLACEHOLDER;
	private String m_dbName		= DB_NAME_PLACEHOLDER;
	private double m_radius		= RADIUS_PLACEHOLDER;
	private int	m_dayShift		= DAY_SHIFT_PLACEHOLDER;

	public String getLogin(){
		return m_login;
	}

	public String getPassword(){
		return m_password;
	}

	public String getServerUrl(){
		return m_serverUrl;
	}

	public String getDbName(){
		return m_dbName;
	}

	public double getRadius() {
		return m_radius;
	}

	public int getDayShift() {
		return m_dayShift;
	}
}
