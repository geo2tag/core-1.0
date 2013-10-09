<?php

include('db_interaction.php');

$reset_password_token=$_GET['token'];

if (empty($reset_password_token))
	die("Token is empty.");

$db_master_connection = getDbConnection();

if ( !checkResetPasswordToken($db_master_connection, $reset_password_token)){
	pg_close($db_master_connection);	
	die("Token is invalid");
}

$new_passsword=generatePassword();


changePasswordForUserWithToken($db_master_connection, $reset_password_token, $new_password);
removeRecordFromResetPasswordTokens($db_master_connection, $reset_password_token)

echo "Your password was reseted successfuly. New passowd: $new_password";

pg_close($db_master_connection);	
?>
