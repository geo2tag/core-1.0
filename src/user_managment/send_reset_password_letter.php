<?php

include('check_captcha.php');
include('db_interaction.php');

$login=$_POST['login'];

if (empty($login))
	die("Incorrect parameters!!");

// Check login existance
$db_master_connection = getMasterDbConnection();

if ( !doesUserExistInUsers( $db_master_connection, $login))
{
	pg_close($db_master_connection);	
	die("User with given login does not exist.");
}

// Add new record into reset_password_tokens
if ( doesResetPasswordTokenExistForLogin ($db_master_connection, $login))
{
	pg_close($db_master_connection);	
	die("User already requested password reset.");
}
$reset_password_token=generateToken();

addResetPasswordToken($db_master_connection, $login, $reset_password_token);
$email=getUserEmailByLogin($db_master_connection, $login);



$subject="Geo2Tag password reset";
$server_address=getServerAddress();
$message="Your reset password link \r\n".
	"Click on this link to reset your password and recieve new one. \r\n".
	"$server_address/check_reset_password_token.php?token=$reset_password_token";

$mail_sending_result = mail($email, $subject, $message);

if ($mail_sending_result)
	echo "Link for password reset was sent to your email.";
else{
	echo "Problems during email sending.";
	removeRecordFromResetPasswordTokens($db_master_connection, $reset_password_token);	
}
pg_close($db_master_connection);
