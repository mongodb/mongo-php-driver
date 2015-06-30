--TEST--
BSON MongoDB\BSON\MinKey #001
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$minkey = new MongoDB\BSON\MinKey;
$tests = array(
    array("min" => $minkey),
);

foreach($tests as $n => $test) {
    $s = MongoDB\BSON\fromArray($test);
    echo "Test#{$n} ", $json = MongoDB\BSON\toJSON($s), "\n";
    $bson = MongoDB\BSON\fromJSON($json);
    $testagain = MongoDB\BSON\toArray($bson);
    var_dump(MongoDB\BSON\toJSON(MongoDB\BSON\fromArray($test)), MongoDB\BSON\toJSON(MongoDB\BSON\fromArray($testagain)));
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
