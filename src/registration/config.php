<?php
$db_host="localhost"; // Host name 
$db_username="geo2tag"; // Mysql username 
$db_password="geo2tag"; // Mysql password 
$default_db_name=getDefaultDbName(); // Database name 

function doesDbExist($db_name, $db_host, $db_username, $db_password)
{
	$master_db="postgres";
	$dbconn = pg_connect("host=$db_host dbname=$master_db user=$db_username password=$db_password")
		or die('Could not connect: ' . pg_last_error());

	$result = pg_query_params("select datname from pg_database where lower(datname) = lower($1);", array($db_name))  
		or die('Could not connect: ' . pg_last_error());
	$line = pg_fetch_array($result, null, PGSQL_ASSOC);
	
	pg_free_result($result);
	pg_close($dbconn);

	if ($line == FALSE)
		return false;

	return true;	
	
}

function getDefaultDbName()
{

	$ini_path = "/opt/geo2tag/geo2tag.conf";
	$ini = parse_ini_file($ini_path, TRUE, INI_SCANNER_RAW);

	$db_section = $ini["database"];

	return $db_section["name"];

}



echo getDefaultDbName();
echo "<br>";
$existance_flag=var_export(doesDbExist("geo12tag",  $db_host, $db_username, $db_password), TRUE);
echo "doesDbExist(geo2tag) == $existance_flag";

?>
