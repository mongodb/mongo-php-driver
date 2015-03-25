--TEST--
BSON BSON\MinKey #001
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$minkey = new BSON\MinKey;
$tests = array(
    array("min" => $minkey),
);

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
Test#0 { "min" : { "$minKey" : 1 } }
string(29) "{ "min" : { "$minKey" : 1 } }"
string(29) "{ "min" : { "$minKey" : 1 } }"
bool(true)
===DONE===
