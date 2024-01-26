<?php
include_once "lib.php";
$db=new db();

$d=$_GET['d'];
$v=$_GET['v'];

echo $d."\n";
echo $v."\n";

$db->act("insert into dev set device='$d', value='$v' ");
$db->finish();
?>
