--TEST--
BSON BSON\Timestamp #001
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$classname = BSON_NAMESPACE . "\\Timestamp";
$timestamp = new $classname(1234, 5678);
$tests = array(
    array("timestamp" => $timestamp),
);

$s = new $classname(1234, 5678);
echo $s, "\n";

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
[1234:5678]
Test#0 { "timestamp" : { "$timestamp" : { "t" : 5678, "i" : 1234 } } }
string(63) "{ "timestamp" : { "$timestamp" : { "t" : 5678, "i" : 1234 } } }"
string(63) "{ "timestamp" : { "$timestamp" : { "t" : 5678, "i" : 1234 } } }"
bool(true)
===DONE===
