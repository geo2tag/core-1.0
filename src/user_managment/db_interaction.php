<?php
$db_host="localhost";  
$db_username="geo2tag"; 
$db_password="geo2tag"; 
$default_db_name=getDefaultDbName(); 


function generateToken(){
	return md5(uniqid(rand()));
}

function generatePassword($length = 8){
	$chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_+=@#$%&*!(){}";
	return substr( str_shuffle( $chars ), 0, $length );	
}

function isQueryResultNotEmpty($result){
	$rows_number = pg_num_rows($result);
	pg_free_result($result);
	return ($rows_number != 0);
}

function doesDbExist($db_name)
{
	global $db_host, $db_username, $db_password;
	$master_db="postgres";
	$dbconn = pg_connect("host=$db_host dbname=$master_db user=$db_username password=$db_password")
		or die("Internal db error.");

	$query_result = pg_query_params($dbconn, 
		"select datname from pg_database where lower(datname) = lower($1);", array($db_name))  
		or die("Internal db error.");
	
	pg_close($dbconn);

	return isQueryResultNotEmpty($query_result);

	
}

function getMasterDbConnection(){
	return getServiceDbConnection(getDefaultDbName());
}

function getServiceDbConnection($db_name){
	global $db_host, $db_username, $db_password;
	$db_connection = pg_connect("host=$db_host dbname=$db_name user=$db_username password=$db_password")
			or die("Internal db error.");

	return $db_connection;
}

function getStringConfigParam($section, $field)
{

	$ini_path = "/opt/geo2tag/geo2tag.conf";
	$ini = parse_ini_file($ini_path, TRUE, INI_SCANNER_RAW);

	$db_section = $ini[$section];

	return $db_section[$field];
}

function getServerAddress()
{
	return getStringConfigParam("common", "server_url");
}

function getDefaultDbName()
{
	return getStringConfigParam("database", "name");
}

// return false if user with $username or $email exists in Users or TmpUsers tables
function doesUserAndTmpUserExist($db_conn, $username, $email)
{
	if ( doesUserExistInTable($db_conn, $username, $email, "users") ) 
		return true;
	if ( doesUserExistInTable($db_conn, $username, $email, "tmp_users")) 
		return true;

	return false;
}

function doesUserExistInUsers($db_conn, $username)
{
	return doesUserExistInTable($db_conn, $username, "", "users");
}

function doesUserExistInTable($db_conn, $username, $email, $table)
{
	$check_user_query_result = pg_query_params($db_conn, 
		"select * from $table where lower(login) = lower($1) ".
		"or lower(email) = lower($2);",
		 array($username, $email))
                	or die("Internal db error.");
	
	return isQueryResultNotEmpty($check_user_query_result);
}

function addNewTmpUser($db_conn, $login, $password, $email, $registration_token, $db_name)
{
	pg_query_params($db_conn, 
                "insert into tmp_users (login, password, email, registration_token, db_name) values ".
                " ($1, $2, $3, $4, $5);", 
                 array($login, $password, $email, $registration_token, $db_name))
			or die("Internal db error.");	


}

function addNewUser($db_conn, $login, $password, $email)
{
	pg_query_params($db_conn, 
                "insert into users (login, password, email) values ".
                " ($1, $2, $3);", 
                 array($login, $password, $email))
			or die("Internal db error.");	


}
// check existance of tmp_user with given token
function doesRegistrationTokenExist($db_conn, $registration_token)
{
	$check_token_query_result = pg_query_params($db_conn, 
		"select * from tmp_users where lower(registration_token) = lower($1); ",
		 array($registration_token))
                	or die("Internal db error.");

	return isQueryResultNotEmpty($check_token_query_result);
}


function getDbNameByRegistrationToken($db_master_connection, $registration_token)
{
	$result_array = getTmpUserByRegistrationToken($db_master_connection, $registration_token);
	return $result_array["db_name"];
}

function getTmpUserByRegistrationToken($db_conn, $registration_token)
{
	$get_tmp_user_query = pg_query_params($db_conn,
		"select login, password, email, db_name from tmp_users ".
		" where lower(registration_token) = lower($1);",
		array($registration_token))
			or die("Internal db error.");
	$result = pg_fetch_array($get_tmp_user_query, null, PGSQL_ASSOC);

	return $result;
}

function getUserByLogin($db_conn, $login)
{
	$get_user_query = pg_query_params($db_conn,
		"select login, password, id, email from users ".
		" where lower(login) = lower($1);",
		array($login))
			or die("Internal db error.");
	$result = pg_fetch_array($get_user_query, null, PGSQL_ASSOC);

	return $result;
}
function getUserEmailByLogin($db_conn, $login)
{
	$result = getUserByLogin($db_conn, $login);

	return $result["email"];
}

//Create new entry in users, move data from tmp users
function convertTmpUser($db_master_conn, $db_service_db_conn, $registration_token)
{
	$tmp_user = getTmpUserByRegistrationToken($db_master_conn, $registration_token);

	$login = $tmp_user["login"];
	$password =$tmp_user["password"];
	$email =$tmp_user["email"];

	addNewUser($db_master_conn, $login, $password, $email);
	addNewUser($db_service_db_conn, $login, $password, $email);

	deleteTmpUser($db_master_conn, $registration_token);

	
}

// Remove user with $registration_token from tmp_user
function deleteTmpUser($db_conn, $registration_token)
{
	pg_query_params($db_conn,
                "delete from tmp_users where lower(registration_token) = lower($1); ",
                 array($registration_token))
                        or die("Internal db error.");
}

function addResetPasswordToken($db_conn, $login, $reset_password_token)
{
	pg_query_params($db_conn,
		"insert into reset_password_tokens (user_id, token) values ".
		" ((select id from users where lower(login)=lower($1)), $2);",
		array($login, $reset_password_token))
			or die("Internal db error."); 
}

function doesResetPasswordTokenExist($db_conn, $reset_password_token)
{
	$check_token_result = pg_query_params($db_conn,
		"select * from reset_password_tokens where lower(token)=lower($1);",
		array($reset_password_token)) or die ("Internal db error.");

        return isQueryResultNotEmpty($check_token_result);
}

function doesResetPasswordTokenExistForLogin($db_conn, $login)
{
	$check_token_result = pg_query_params($db_conn,
		"select * from reset_password_tokens where user_id = ".
		"( select id from users where lower(login)=lower($1) );",
		array($login)) or die ("Internal db error.");

        return isQueryResultNotEmpty($check_token_result);
}

function changePasswordForUserWithToken($db_conn, $reset_password_token, $new_password)
{
	pg_query_params("update users set password=$1 ".
		" where id=(select user_id from reset_password_tokens where lower(token)=lower($2));",
		array($new_password,  $reset_password_token)) or die("Internal db error.");

}

function removeRecordFromResetPasswordTokens($db_conn, $reset_password_token)
{
	pg_query_params("delete from reset_password_tokens where lower(token)=lower($1);",
		array($reset_password_token)) or die ("Internal db error.");
}

function areCredentialsCorrect($db_conn, $login, $password)
{
	$check_user_result = pg_query_params($db_conn,
		"select * from users where lower(login)=lower($1) and  ".
		" password=$2;",
		array($login, $password)) or die ("Internal db error.");

        return isQueryResultNotEmpty($check_user_result);
}

?>
