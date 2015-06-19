--TEST--
BSON MongoDB\BSON\ObjectID #001
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$sameid = new MongoDB\BSON\ObjectID("53e2a1c40640fd72175d4603");
$samestd = new stdClass;
$samestd->my = $sameid;
$samearr = array("my" => $sameid);


$std = new stdclass;
$std->_id = new MongoDB\BSON\ObjectID;

$array = array(
    "_id" => new MongoDB\BSON\ObjectID,
    "id" => new MongoDB\BSON\ObjectID,
    "d" => new MongoDB\BSON\ObjectID,
);

$pregenerated = new MongoDB\BSON\ObjectID("53e28b650640fd3162152de1");

$tests = array(
    $array,
    $std,
    $samestd,
    $samearr,
    array("pregenerated" => $pregenerated),
);

foreach($tests as $n => $test) {
    $s = MongoDB\BSON\fromArray($test);
    echo "Test#{$n} ", $json = MongoDB\BSON\toJSON($s), "\n";
    $bson = MongoDB\BSON\fromJSON($json);
    $testagain = MongoDB\BSON\toArray($bson);
    var_dump(MongoDB\BSON\toJSON(MongoDB\BSON\fromArray($test)), MongoDB\BSON\toJSON(MongoDB\BSON\fromArray($testagain)));
    var_dump((object)$test == (object)$testagain);
}

throws(function() {
$id = new MongoDB\BSON\ObjectID("53e28b650640fd3162152de12");
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");
throws(function() {
$id = new MongoDB\BSON\ObjectID("53e28b650640fd3162152dg1");
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");
throws(function() {
$id = new MongoDB\BSON\ObjectID("-3e28b650640fd3162152da1");
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");
throws(function() {
$id = new MongoDB\BSON\ObjectID(" 3e28b650640fd3162152da1");
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");
throws(function() {
$id = new MongoDB\BSON\ObjectID(new stdclass);
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");



raises(function() use($pregenerated) {
    $pregenerated->__toString(1);
}, E_WARNING);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Test#0 { "_id" : { "$oid" : "%s" }, "id" : { "$oid" : "%s" }, "d" : { "$oid" : "%s" } }
string(146) "{ "_id" : { "$oid" : "%s" }, "id" : { "$oid" : "%s" }, "d" : { "$oid" : "%s" } }"
string(146) "{ "_id" : { "$oid" : "%s" }, "id" : { "$oid" : "%s" }, "d" : { "$oid" : "%s" } }"
bool(true)
Test#1 { "_id" : { "$oid" : "%s" } }
string(51) "{ "_id" : { "$oid" : "%s" } }"
string(51) "{ "_id" : { "$oid" : "%s" } }"
bool(true)
Test#2 { "my" : { "$oid" : "53e2a1c40640fd72175d4603" } }
string(50) "{ "my" : { "$oid" : "53e2a1c40640fd72175d4603" } }"
string(50) "{ "my" : { "$oid" : "53e2a1c40640fd72175d4603" } }"
bool(true)
Test#3 { "my" : { "$oid" : "53e2a1c40640fd72175d4603" } }
string(50) "{ "my" : { "$oid" : "53e2a1c40640fd72175d4603" } }"
string(50) "{ "my" : { "$oid" : "53e2a1c40640fd72175d4603" } }"
bool(true)
Test#4 { "pregenerated" : { "$oid" : "53e28b650640fd3162152de1" } }
string(60) "{ "pregenerated" : { "$oid" : "53e28b650640fd3162152de1" } }"
string(60) "{ "pregenerated" : { "$oid" : "53e28b650640fd3162152de1" } }"
bool(true)
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
OK: Got E_WARNING
===DONE===
