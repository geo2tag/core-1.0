<?php

include('config.php');

// Passkey that got from link 
$registration_token=$_GET['passkey'];

if (empty($registration_token))
	die("Token is empty.");


$db_master_connection = pg_connect("host=$db_host dbname=$default_db_name user=$db_username password=$db_password")
                or die("Internal db error.");

if ( !checkTokenExistance($db_master_connection, $registration_token)){
	pg_close($db_master_connection);	
	die("Token is invalid");
}

$db_name = getDbNameByToken($db_master_connection, $registration_token);

$db_service_connection = pg_connect("host=$db_host dbname=$db_name user=$db_username password=$db_password")
                or die("Internal db error.");

convertTmpUser($db_master_connection, $db_service_connection, $registration_token);

echo "Congratulations! User was confirmed successfuly.";

pg_close($db_master_connection);	
pg_close($db_service_connection);	
?>
