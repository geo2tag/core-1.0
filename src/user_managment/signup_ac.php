<?php


/*require_once('recaptchalib.php');
$key_file="/opt/geo2tag/recaptcha_pkey";
if (!file_exists($key_file))
	die("Internal error!!!");

$private_key = file_get_contents($key_file);
$resp = recaptcha_check_answer ($private_key,
	$_SERVER["REMOTE_ADDR"],
	$_POST["recaptcha_challenge_field"],
	$_POST["recaptcha_response_field"]);

if (!$resp->is_valid) 
	die ("The reCAPTCHA wasn't entered correctly. Go back and try it again.");*/

session_start();
include_once $_SERVER['DOCUMENT_ROOT'] . '/securimage/securimage.php';
$securimage = new Securimage();
if ($securimage->check($_POST['captcha_code']) == false) 
	die ("The CAPTCHA wasn't entered correctly. Go back and try it again.");

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


$registration_token = md5(uniqid(rand())); 

// Opening connection to master db and db with db_name 
$db_master_connection = pg_connect("host=$db_host dbname=$default_db_name user=$db_username password=$db_password")
                or die("Internal db error.");

$db_service_connection = pg_connect("host=$db_host dbname=$db_name user=$db_username password=$db_password")
                or die("Internal db error.");

if ( checkUserExistance( $db_master_connection, $login, $email))
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
