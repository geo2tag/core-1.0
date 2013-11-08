<?php

include('db_interaction.php');

// Passkey that got from link 
$registration_token=$_GET['passkey'];

if (empty($registration_token))
	die("Token is empty.");


$db_master_connection = getMasterDbConnection();

if ( !doesRegistrationTokenExist($db_master_connection, $registration_token)){
	pg_close($db_master_connection);	
	die("Token is invalid");
}

$db_name = getDbNameByRegistrationToken($db_master_connection, $registration_token);

$db_service_connection = getServiceDbConnection($db_name);
 
convertTmpUser($db_master_connection, $db_service_connection, $registration_token);

echo "Congratulations! User was confirmed successfuly.";

pg_close($db_master_connection);	
pg_close($db_service_connection);	
?>
