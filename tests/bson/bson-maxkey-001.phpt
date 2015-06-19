--TEST--
BSON MongoDB\BSON\MaxKey #001
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$maxkey = new MongoDB\BSON\MaxKey;
$tests = array(
    array("max" => $maxkey),
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
Test#0 { "max" : { "$maxKey" : 1 } }
string(29) "{ "max" : { "$maxKey" : 1 } }"
string(29) "{ "max" : { "$maxKey" : 1 } }"
bool(true)
===DONE===
