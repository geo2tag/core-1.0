package org.geo2tag.stubproject.test;

import org.geo2tag.stubproject.SERVICE_NAME_PLACEHOLDER;

import android.content.Intent;
import android.test.ServiceTestCase;

public class SERVICE_NAME_PLACEHOLDERTest extends ServiceTestCase<SERVICE_NAME_PLACEHOLDER> {

	private Intent m_intent;
	private SERVICE_NAME_PLACEHOLDER m_service;
	
	public SERVICE_NAME_PLACEHOLDERTest() {
		super(SERVICE_NAME_PLACEHOLDER.class);
	}
	
	public SERVICE_NAME_PLACEHOLDERTest(Class<SERVICE_NAME_PLACEHOLDER> serviceClass) {
		super(serviceClass);
		// TODO Auto-generated constructor stub
	}
	
	@Override
	protected void setUp() throws Exception{
		super.setUp();
		m_intent = new Intent(this.getSystemContext(),SERVICE_NAME_PLACEHOLDER.class);
	}


	public void testOnCreate() {
		this.startService(m_intent);
				
		m_service = this.getService();
		
		assert(m_service.isServiceUp()); 
		assertNotNull(m_service.getSettings());
		
	}
	
	public void testOnDestroy(){
		this.shutdownService();
		m_service = this.getService();
		
		assert(!m_service.isServiceUp()); 
		
	}
	

}
