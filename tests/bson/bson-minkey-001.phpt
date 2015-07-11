--TEST--
BSON BSON\MinKey #001
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$classname = BSON_NAMESPACE . "\\MinKey";
$minkey = new $classname;
$tests = array(
    array("min" => $minkey),
);

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
Test#0 { "min" : { "$minKey" : 1 } }
string(29) "{ "min" : { "$minKey" : 1 } }"
string(29) "{ "min" : { "$minKey" : 1 } }"
bool(true)
===DONE===
