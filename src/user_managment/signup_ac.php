<?php

include('check_captcha.php');
include('db_interaction.php');


$login=$_POST['name'];
$password=$_POST['password'];
$email=$_POST['email'];
$db_name=$_POST['db_name'];
$age_restriction=$_POST['age_restriction'];

if (empty($login) || empty($password) || empty($email) || empty($db_name))
	die("Incorrect parameters!!");

if ($age_restriction != "age_restriction")
	die ("Age restriction is not met.");

if ( ! doesDbExist($db_name)) 
	die("Service with given name does not exist!!!");


$registration_token = generateToken(); 

// Opening connection to master db and db with db_name 
$db_master_connection = getMasterDbConnection();

$db_service_connection = getServiceDbConnection($db_name);

if ( doesUserAndTmpUserExist( $db_master_connection, $login, $email))
{
	pg_close($db_master_connection);	
	pg_close($db_service_connection);	
	die("User already exists!!!");
}


addNewTmpUser($db_master_connection, $login, $password, $email, $registration_token, $db_name);

$subject="Geo2Tag registration confirmation";
$server_address=getServerAddress();
$message="Your Comfirmation link \r\n".
	"Click on this link to activate your account \r\n".
	"$server_address/confirmation.php?passkey=$registration_token";

$mail_sending_result = mail($email, $subject, $message);

if ($mail_sending_result)
{
	echo "Confirmation link was sent on your email address.";
}else{
	echo "Problems during email sending.";
}

pg_close($db_master_connection);	
pg_close($db_service_connection);	
?>
