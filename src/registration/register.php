<html>
<head>
<title>Geo2Tag services registration</title>
</head>

<body>
<table width="350" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><form name="form1" method="post" action="signup_ac.php">
<table width="100%" border="0" cellspacing="4" cellpadding="0">
<tr>
<td colspan="3"><strong>Sign up</strong></td>
</tr>
<tr>
<td >Name</td>
<td >:</td>
<td ><input name="name" type="text" id="name" size="30"></td>
</tr>
<tr>
<td>E-mail</td>
<td>:</td>
<td><input name="email" type="text" id="email" size="30"></td>
</tr>
<tr>
<td>Password</td>
<td>:</td>
<td><input name="password" type="password" id="password" size="30"></td>
</tr>
<?php
$db_name = $_GET["db_name"];
if (empty($db_name))
{
	echo "<tr>";
	echo "<td>Db name</td>";
	echo "<td>:</td>";
	echo '<td><input name="db_name" type="text" id="db_name" size="30"></td>';
	echo "</tr>";
}else{
	echo "<input name=\"db_name\" type=\"hidden\" id=\"db_name\" value=\"$db_name\">";
}
?>
<tr><td>
<?php
	require_once('recaptchalib.php');
	$publickey = "6LdHlucSAAAAAIRBeeKVLR_nmvyCbDX064F08JJx"; 
	echo recaptcha_get_html($publickey);
?>
</td></tr>
<tr>
<td><input type="submit" name="Submit" value="Submit">&nbsp;
<input type="reset" name="Reset" value="Reset"></td>
</tr>
</table>
</form></td>
</tr>
</table>

</body>
</html>
