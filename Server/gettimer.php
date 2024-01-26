<?php
include_once "lib.php";
$db=new db();

$d=$_GET['d'];
$v=$_GET['timer'];

echo "<html>
	<head>
	<title> Setup Timer </title>
	<link rel='preconnect' href='https://fonts.googleapis.com'>
	<link rel='preconnect' href='https://fonts.gstatic.com' crossorigin>
	<link href='https://fonts/googleapis.com/css2?family=Merienda:wght@700&family=Open+Sans:ital,wght@1,300&display=swap' rel='stylesheet'>
	<style>
	*{
		font-family:'Merienda', cursive;
		font-family:'Open Sans', sans-serif;
	}
	</style>
	</head>
	<body>
		<center><h1> Set Time </h1></center>
		<center><h2> Title : $d, time : $v </h2></center>
		<center>
		<a href='index.php'> go to main </a>
		</center>
	</body>
	</html>";

$db->act("insert into timer set title='$d', time='$v' ");
$db->finish();
?>

