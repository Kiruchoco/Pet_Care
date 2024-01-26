<?php
include_once "lib.php";
$db=new db();

$db->get("select * from timer ", $rs, $rn);
//print_r($rs);

for($i=0;$i<$rn;$i++)
{
	echo $rs[$i]['seq']." ".$rs[$i]['title']." ".$rs[$i]['time']."<br>\n";
}

$db->finish();
?>
