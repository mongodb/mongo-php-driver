--TEST--
BSON BSON\Binary #001
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php 
require_once "tests/utils/basic.inc";

$types = array(
    BSON\Binary::TYPE_GENERIC,
    BSON\Binary::TYPE_FUNCTION,
    BSON\Binary::TYPE_OLD_BINARY,
    BSON\Binary::TYPE_OLD_UUID,
    BSON\Binary::TYPE_UUID,
    BSON\Binary::TYPE_MD5,
    BSON\Binary::TYPE_USER_DEFINED,
    BSON\Binary::TYPE_USER_DEFINED+5,
);
$tests = array();
foreach($types as $type) {
    $binary = new BSON\Binary("random binary data", $type);
    var_dump($binary->getSubType() == $type);
    $tests[] = array("binary" => $binary);
}

throws(function() {
    $b = new BSON\Binary("random binary data without type", BSON\Binary::TYPE_USER_DEFINED);
    $b->getSubType($b);
}, "InvalidArgumentException", "getSubType");

throws(function() {
    $b = new BSON\Binary("random binary data without type");
    echo "FAIL: Constructed BSON\Binary without type!\n";
}, "InvalidArgumentException");



foreach($tests as $n => $test) {
    $s = BSON\fromArray($test);
    echo "Test#{$n} ", $json = BSON\toJSON($s), "\n";
    $bson = BSON\fromJSON($json);
    $testagain = BSON\toArray($bson);
    var_dump(BSON\toJSON(BSON\fromArray($test)), BSON\toJSON(BSON\fromArray($testagain)));
    var_dump((object)$test == (object)$testagain);
}
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
OK: Got InvalidArgumentException thrown from getSubType
OK: Got InvalidArgumentException
Test#0 { "binary" : { "$type" : "00", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }
string(73) "{ "binary" : { "$type" : "00", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }"
string(73) "{ "binary" : { "$type" : "00", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }"
bool(true)
Test#1 { "binary" : { "$type" : "01", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }
string(73) "{ "binary" : { "$type" : "01", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }"
string(73) "{ "binary" : { "$type" : "01", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }"
bool(true)
Test#2 { "binary" : { "$type" : "02", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }
string(73) "{ "binary" : { "$type" : "02", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }"
string(73) "{ "binary" : { "$type" : "02", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }"
bool(true)
Test#3 { "binary" : { "$type" : "03", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }
string(73) "{ "binary" : { "$type" : "03", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }"
string(73) "{ "binary" : { "$type" : "03", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }"
bool(true)
Test#4 { "binary" : { "$type" : "04", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }
string(73) "{ "binary" : { "$type" : "04", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }"
string(73) "{ "binary" : { "$type" : "04", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }"
bool(true)
Test#5 { "binary" : { "$type" : "05", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }
string(73) "{ "binary" : { "$type" : "05", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }"
string(73) "{ "binary" : { "$type" : "05", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }"
bool(true)
Test#6 { "binary" : { "$type" : "80", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }
string(73) "{ "binary" : { "$type" : "80", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }"
string(73) "{ "binary" : { "$type" : "80", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }"
bool(true)
Test#7 { "binary" : { "$type" : "85", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }
string(73) "{ "binary" : { "$type" : "85", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }"
string(73) "{ "binary" : { "$type" : "85", "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh" } }"
bool(true)
===DONE===
