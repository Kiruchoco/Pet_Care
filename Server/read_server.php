<?php
include_once "lib.php";
$db=new db();

$d=$_GET['d'];
$db->get("select time from timer order by seq desc limit 1 ",$rs, $rn);

echo $rs[0]['time'];

$db->finish();
?>
