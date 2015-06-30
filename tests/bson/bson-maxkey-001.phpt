--TEST--
BSON BSON\MaxKey #001
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$classname = BSON_NAMESPACE . "\\MaxKey";
$maxkey = new $classname;
$tests = array(
    array("max" => $maxkey),
);

foreach($tests as $n => $test) {
    $s = fromArray($test);
    echo "Test#{$n} ", $json = toJSON($s), "\n";
    $bson = fromJSON($json);
    $testagain = toArray($bson);
    var_dump(toJSON(fromArray($test)), toJSON(fromArray($testagain)));
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
