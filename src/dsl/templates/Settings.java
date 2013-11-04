
package org.geo2tag.stubproject;


class Settings
{

	private static Settings m_instance = null;

	public static Settings getInstance(){
		if (m_instance == null) m_instance = new Settings();

		return m_instance;
	}


	private String m_login		= null;
	private String m_password	= null;
	private String m_serverUrl	= "SERVER_URL_PLACEHOLDER";
	private String m_dbName		= "DB_NAME_PLACEHOLDER";
	private double m_radius		= 10;
	private int	m_dayShift	= 1;

	public String getLogin(){
		return m_login;
	}

	public void setLogin(String login){
		m_login = login;
	}

	public String getPassword(){
		return m_password;
	}

	public void setPassword(String password){
		m_password = password;
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
