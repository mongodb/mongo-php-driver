--TEST--
BSON BSON\Timestamp #001
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$timestamp = new BSON\Timestamp(1234, 5678);
$tests = array(
    array("timestamp" => $timestamp),
);

throws(function() {
    $s = new BSON\Timestamp;
}, "InvalidArgumentException");

$s = new BSON\Timestamp(1234, 5678);
echo $s, "\n";

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
OK: Got InvalidArgumentException
[1234:5678]
Test#0 { "timestamp" : { "$timestamp" : { "t" : 5678, "i" : 1234 } } }
string(63) "{ "timestamp" : { "$timestamp" : { "t" : 5678, "i" : 1234 } } }"
string(63) "{ "timestamp" : { "$timestamp" : { "t" : 5678, "i" : 1234 } } }"
bool(true)
===DONE===
