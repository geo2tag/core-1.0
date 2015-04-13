package org.geo2tag.stubproject;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.content.Intent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;


public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		
		Button goToMapActivityButton = (Button)findViewById(R.id.move_to_map);
		goToMapActivityButton.setOnClickListener( new OnClickListener(){

			@Override
			public void onClick(View v) {
				//MAP_ACTIVITY_TRANSFER_PLACEHOLDER	
			}	
		});
		
		final EditText loginEdit = (EditText)findViewById(R.id.edit_login);
		final EditText passwordEdit = (EditText)findViewById(R.id.edit_password);
		Button loginButton = (Button)findViewById(R.id.login);
		loginButton.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v){
				Settings.getInstance().setLogin(loginEdit.getText().toString());
				Settings.getInstance().setPassword(passwordEdit.getText().toString());
			}
		});
		
	}

}
