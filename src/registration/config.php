<?php
$db_host="localhost";  
$db_username="geo2tag"; 
$db_password="geo2tag"; 
$default_db_name=getDefaultDbName(); 

function doesDbExist($db_name)
{
	global $db_host, $db_username, $db_password;
	$master_db="postgres";
	$dbconn = pg_connect("host=$db_host dbname=$master_db user=$db_username password=$db_password")
		or die("Internal db error.");

	$query_result = pg_query_params($dbconn, "select datname from pg_database where lower(datname) = lower($1);", array($db_name))  
		or die("Internal db error.");
	$line = pg_fetch_array($query_result, null, PGSQL_ASSOC);
	
	pg_free_result($query_result);
	pg_close($dbconn);

	$result=(bool)($line != FALSE);

	return $result;

	
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
function checkUserExistance($db_conn, $username, $email)
{
	if ( checkUserExistanceInTable($db_conn, $username, $email, "users") ) 
		return true;
	if ( checkUserExistanceInTable($db_conn, $username, $email, "tmp_users")) 
		return true;

	return false;
}

function checkUserExistanceInTable($db_conn, $username, $email, $table)
{
	$check_user_query_result = pg_query_params($db_conn, 
		"select * from $table where lower(login) = lower($1) ".
		"or lower(email) = lower($2);",
		 array($username, $email))
                	or die("Internal db error.");

	$row_count = pg_num_rows($check_user_query_result);
	pg_free_result($check_user_query_result);
	
	return ((bool)($row_count != 0));
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
function checkTokenExistance($db_conn, $registration_token)
{
	$check_token_query_result = pg_query_params($db_conn, 
		"select * from tmp_users where lower(registration_token) = lower($1); ",
		 array($registration_token))
                	or die("Internal db error.");

	$row_count = pg_num_rows($check_token_query_result);
	pg_free_result($check_token_query_result);
	return ((bool)($row_count != 0));
}


function getDbNameByToken($db_master_connection, $registration_token)
{
	$result_array = getTmpUser($db_master_connection, $registration_token);
	return $result_array["db_name"];
}

function getTmpUser($db_conn, $registration_token)
{
	$get_tmp_user_query = pg_query_params($db_conn,
		"select login, password, email, db_name from tmp_users ".
		" where lower(registration_token) = lower($1);",
		array($registration_token))
			or die("Internal db error.");
	$result = pg_fetch_array($get_tmp_user_query, null, PGSQL_ASSOC);

	return $result;
}

//Create new entry in users, move data from tmp users
function convertTmpUser($db_master_conn, $db_service_db_conn, $registration_token)
{
	$tmp_user = getTmpUser($db_master_conn, $registration_token);

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

/*
echo getDefaultDbName();
echo "<br>";
$db_name_to_check="geo12tag";
$existance_flag=var_export(doesDbExist($db_name_to_check), TRUE);
echo "doesDbExist($db_name_to_check) == $existance_flag";*/

?>
