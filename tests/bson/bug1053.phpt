--TEST--
PHPC-1053: MongoDB\BSON\UTCDateTime's constructor has argument defined as required
--FILE--
<?php
$reflection = new ReflectionMethod(MongoDB\BSON\UTCDateTime::class, '__construct');
var_dump($reflection->getParameters()[0]->isOptional());
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===
