--TEST--
BSON BSON\MaxKey #001
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php 
require_once "tests/utils/basic.inc";

$maxkey = new BSON\MaxKey;
$tests = array(
    array("max" => $maxkey),
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
Test#0 { "max" : { "$maxKey" : 1 } }
string(29) "{ "max" : { "$maxKey" : 1 } }"
string(29) "{ "max" : { "$maxKey" : 1 } }"
bool(true)
