<?

session_start();
include_once $_SERVER['DOCUMENT_ROOT'] . '/securimage/securimage.php';
$securimage = new Securimage();
if ($securimage->check($_POST['captcha_code']) == false) 
	die ("The CAPTCHA wasn't entered correctly. Go back and try it again.");
?>
