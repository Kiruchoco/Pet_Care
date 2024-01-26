<?php
include_once "lib.php";
$db=new db();

$db->get("select * from dev ", $rs, $rn);
//print_r($rs);

for($i=0;$i<$rn;$i++)
{
	echo $rs[$i]['seq']." ".$rs[$i]['device']." ".$rs[$i]['value']."<br>\n";
}

$db->finish();
?>
