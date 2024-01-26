<?php
include_once "lib.php";
$db=new db();

$d=$_GET['d'];
$db->get("select title,time from timer order by seq desc limit 1 ",$rs, $rn);

echo "<b>".$rs[0]['title']." ".$rs[0]['time']."\n</b>";

echo "<html>
	<head>
	</head>
	<body>
		<form method='get' action='gettimer.php'>
		<p><label>Enter title : <input type='text' name='d'></label></p>
		<p><label>Select time : <input type='time' name='timer'></label></p>
		<p><input type='submit' value='Submit'></p>
		</form>
	</body>
	</html>";

$db->finish();
?>
