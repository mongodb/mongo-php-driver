--TEST--
BSON BSON\ObjectID #001
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$sameid = new BSON\ObjectID("53e2a1c40640fd72175d4603");
$samestd = new stdClass;
$samestd->my = $sameid;
$samearr = array("my" => $sameid);


$std = new stdclass;
$std->_id = new BSON\ObjectID;

$array = array(
    "_id" => new BSON\ObjectID,
    "id" => new BSON\ObjectID,
    "d" => new BSON\ObjectID,
);

$pregenerated = new BSON\ObjectID("53e28b650640fd3162152de1");

$tests = array(
    $array,
    $std,
    $samestd,
    $samearr,
    array("pregenerated" => $pregenerated),
);

foreach($tests as $n => $test) {
    $s = BSON\fromArray($test);
    echo "Test#{$n} ", $json = BSON\toJSON($s), "\n";
    $bson = BSON\fromJSON($json);
    $testagain = BSON\toArray($bson);
    var_dump(BSON\toJSON(BSON\fromArray($test)), BSON\toJSON(BSON\fromArray($testagain)));
    var_dump((object)$test == (object)$testagain);
}

throws(function() {
$id = new BSON\ObjectID("53e28b650640fd3162152de12");
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");
throws(function() {
$id = new BSON\ObjectID("53e28b650640fd3162152dg1");
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");
throws(function() {
$id = new BSON\ObjectID("-3e28b650640fd3162152da1");
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");
throws(function() {
$id = new BSON\ObjectID(" 3e28b650640fd3162152da1");
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");
throws(function() {
$id = new BSON\ObjectID(new stdclass);
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
