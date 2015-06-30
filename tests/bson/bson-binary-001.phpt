--TEST--
BSON BSON\Binary #001
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$types = array(
    MongoDB\BSON\Binary::TYPE_GENERIC,
    MongoDB\BSON\Binary::TYPE_FUNCTION,
    MongoDB\BSON\Binary::TYPE_OLD_BINARY,
    MongoDB\BSON\Binary::TYPE_OLD_UUID,
    MongoDB\BSON\Binary::TYPE_UUID,
    MongoDB\BSON\Binary::TYPE_MD5,
    MongoDB\BSON\Binary::TYPE_USER_DEFINED,
    MongoDB\BSON\Binary::TYPE_USER_DEFINED+5,
);
$tests = array();
foreach($types as $type) {
    $binary = new MongoDB\BSON\Binary("random binary data", $type);
    var_dump($binary->getSubType() == $type);
    $tests[] = array("binary" => $binary);
}

throws(function() {
    $b = new MongoDB\BSON\Binary("random binary data without type");
    echo "FAIL: Constructed MongoDB\BSON\Binary without type!\n";
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");



foreach($tests as $n => $test) {
    $s = MongoDB\BSON\fromArray($test);
    echo "Test#{$n} ", $json = MongoDB\BSON\toJSON($s), "\n";
    $bson = MongoDB\BSON\fromJSON($json);
    $testagain = MongoDB\BSON\toArray($bson);
    var_dump(MongoDB\BSON\toJSON(MongoDB\BSON\fromArray($test)), MongoDB\BSON\toJSON(MongoDB\BSON\fromArray($testagain)));
    var_dump((object)$test == (object)$testagain);
}

$binary->getSubType(2);
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
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
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

Warning: MongoDB\BSON\Binary::getSubType() expects exactly 0 parameters, 1 given in %s on line %d
===DONE===
