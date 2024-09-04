--TEST--
MongoDB\BSON\DBPointer comparisons
--FILE--
<?php

$jsonTest0 =  MongoDB\BSON\Document::fromJSON('{ "dbref": {"$dbPointer": {"$ref": "phongo.test", "$id" : { "$oid" : "5a2e78accd485d55b4050000" }  }} }')->toPHP();
$jsonTest1a = MongoDB\BSON\Document::fromJSON('{ "dbref": {"$dbPointer": {"$ref": "phongo.test", "$id" : { "$oid" : "5a2e78accd485d55b4051111" }  }} }')->toPHP();
$jsonTest1b = MongoDB\BSON\Document::fromJSON('{ "dbref": {"$dbPointer": {"$ref": "phongo.test", "$id" : { "$oid" : "5a2e78accd485d55b4051111" }  }} }')->toPHP();
$jsonTest2 =  MongoDB\BSON\Document::fromJSON('{ "dbref": {"$dbPointer": {"$ref": "phongo.test", "$id" : { "$oid" : "5a2e78accd485d55b4052222" }  }} }')->toPHP();
$jsonAAAA =   MongoDB\BSON\Document::fromJSON('{ "dbref": {"$dbPointer": {"$ref": "phongo.aaaa", "$id" : { "$oid" : "5a2e78accd485d55b4051111" }  }} }')->toPHP();
$jsonZZZZ =   MongoDB\BSON\Document::fromJSON('{ "dbref": {"$dbPointer": {"$ref": "phongo.zzzz", "$id" : { "$oid" : "5a2e78accd485d55b4051111" }  }} }')->toPHP();

var_dump($jsonTest1a == $jsonTest1b);
var_dump($jsonTest0 < $jsonTest1b);
var_dump($jsonTest2 > $jsonTest1b);
var_dump($jsonAAAA < $jsonTest1b);
var_dump($jsonZZZZ > $jsonTest1b);
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
===DONE===
