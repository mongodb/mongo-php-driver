--TEST--
BSON BSON\ObjectID #001
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$classname = BSON_NAMESPACE . "\\ObjectID";
$sameid = new $classname("53e2a1c40640fd72175d4603");
$samestd = new stdClass;
$samestd->my = $sameid;
$samearr = array("my" => $sameid);


$std = new stdclass;
$std->_id = new $classname;

$array = array(
    "_id" => new $classname,
    "id" => new $classname,
    "d" => new $classname,
);

$pregenerated = new $classname("53e28b650640fd3162152de1");

$tests = array(
    $array,
    $std,
    $samestd,
    $samearr,
    array("pregenerated" => $pregenerated),
);

foreach($tests as $n => $test) {
    $s = fromPHP($test);
    echo "Test#{$n} ", $json = toJSON($s), "\n";
    $bson = fromJSON($json);
    $testagain = toPHP($bson);
    var_dump(toJSON(fromPHP($test)), toJSON(fromPHP($testagain)));
    var_dump((object)$test == (object)$testagain);
}

throws(function() use($classname) {
$id = new $classname("53e28b650640fd3162152de12");
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");
throws(function() use($classname) {
$id = new $classname("53e28b650640fd3162152dg1");
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");
throws(function() use ($classname) {
$id = new $classname("-3e28b650640fd3162152da1");
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");
throws(function() use($classname) {
$id = new $classname(" 3e28b650640fd3162152da1");
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
OK: Got E_WARNING
===DONE===
