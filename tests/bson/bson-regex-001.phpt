--TEST--
BSON BSON\Regex #001
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php 
require_once "tests/utils/basic.inc";

$regexp = new BSON\Regex("regexp", "i");
printf("Pattern: %s\n", $regexp->getPattern());
printf("Flags: %s\n", $regexp->getFlags());
printf("String representation: %s\n", $regexp);

$tests = array(
    array("regex" => $regexp),
);

throws(function() {
    $regexp = new BSON\Regex;
}, "InvalidArgumentException");
throws(function() {
    $regexp = new BSON\Regex("regexp", "i");
    $regexp->getPattern(1);
}, "InvalidArgumentException", "getPattern");
throws(function() {
    $regexp = new BSON\Regex("regexp", "i");
    $regexp->getFlags(1);
}, "InvalidArgumentException", "getFlags");

throws(function() {
    $regexp = new BSON\Regex("regexp", "i");
    $regexp->__toString(1);
}, "InvalidArgumentException", "__toString");


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
Pattern: regexp
Flags: i
String representation: /regexp/i
OK: Got InvalidArgumentException
OK: Got InvalidArgumentException thrown from getPattern
OK: Got InvalidArgumentException thrown from getFlags
OK: Got InvalidArgumentException thrown from __toString
Test#0 { "regex" : { "$regex" : "regexp", "$options" : "i" } }
string(55) "{ "regex" : { "$regex" : "regexp", "$options" : "i" } }"
string(55) "{ "regex" : { "$regex" : "regexp", "$options" : "i" } }"
bool(true)
===DONE===
