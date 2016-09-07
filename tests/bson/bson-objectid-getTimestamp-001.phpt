--TEST--
MongoDB\BSON\ObjectID::getTimestamp
--FILE--
<?php
date_default_timezone_set( "UTC" );

$objectid = new MongoDB\BSON\ObjectID('564b07d6619d4b3d668b45e3');
$ts = $objectid->getTimestamp();
echo $ts, "\n";
echo date_create( "@{$ts}" )->format( "Y-m-d H:i:s" ), "\n";
?>
--EXPECTF--
1447757782
2015-11-17 10:56:22
