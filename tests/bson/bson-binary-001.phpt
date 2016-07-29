--TEST--
BSON BSON\Binary #001
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$classname = BSON_NAMESPACE . "\\Binary";

$types = array(
    $classname::TYPE_GENERIC,
    $classname::TYPE_FUNCTION,
    $classname::TYPE_OLD_BINARY,
    $classname::TYPE_OLD_UUID,
    $classname::TYPE_UUID,
    $classname::TYPE_MD5,
    $classname::TYPE_USER_DEFINED,
    $classname::TYPE_USER_DEFINED+5,
);
$tests = array();
foreach($types as $type) {
    $binary = new $classname("random binary data", $type);
    var_dump($binary->getData() == "random binary data");
    var_dump($binary->getType() == $type);
    $tests[] = array("binary" => $binary);
}

foreach($tests as $n => $test) {
    $s = fromPHP($test);
    echo "Test#{$n} ", $json = toJSON($s), "\n";
    $bson = fromJSON($json);
    $testagain = toPHP($bson);
    var_dump(toJSON(fromPHP($test)), toJSON(fromPHP($testagain)));
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
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
Test#0 { "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "00" } }
string(73) "{ "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "00" } }"
string(73) "{ "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "00" } }"
bool(true)
Test#1 { "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "01" } }
string(73) "{ "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "01" } }"
string(73) "{ "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "01" } }"
bool(true)
Test#2 { "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "02" } }
string(73) "{ "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "02" } }"
string(73) "{ "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "02" } }"
bool(true)
Test#3 { "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "03" } }
string(73) "{ "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "03" } }"
string(73) "{ "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "03" } }"
bool(true)
Test#4 { "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "04" } }
string(73) "{ "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "04" } }"
string(73) "{ "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "04" } }"
bool(true)
Test#5 { "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "05" } }
string(73) "{ "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "05" } }"
string(73) "{ "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "05" } }"
bool(true)
Test#6 { "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "80" } }
string(73) "{ "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "80" } }"
string(73) "{ "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "80" } }"
bool(true)
Test#7 { "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "85" } }
string(73) "{ "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "85" } }"
string(73) "{ "binary" : { "$binary" : "cmFuZG9tIGJpbmFyeSBkYXRh", "$type" : "85" } }"
bool(true)
===DONE===
