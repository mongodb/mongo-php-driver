--TEST--
BSON MongoDB\BSON\Regex #001
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$regexp = new MongoDB\BSON\Regex("regexp", "i");
printf("Pattern: %s\n", $regexp->getPattern());
printf("Flags: %s\n", $regexp->getFlags());
printf("String representation: %s\n", $regexp);

$tests = array(
    array("regex" => $regexp),
);

throws(function() {
    $regexp = new MongoDB\BSON\Regex;
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");


foreach($tests as $n => $test) {
    $s = MongoDB\BSON\fromArray($test);
    echo "Test#{$n} ", $json = MongoDB\BSON\toJSON($s), "\n";
    $bson = MongoDB\BSON\fromJSON($json);
    $testagain = MongoDB\BSON\toArray($bson);
    var_dump(MongoDB\BSON\toJSON(MongoDB\BSON\fromArray($test)), MongoDB\BSON\toJSON(MongoDB\BSON\fromArray($testagain)));
    var_dump((object)$test == (object)$testagain);
}

$regexp->getPattern(true);
$regexp->getFlags(true);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Pattern: regexp
Flags: i
String representation: /regexp/i
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Test#0 { "regex" : { "$regex" : "regexp", "$options" : "i" } }
string(55) "{ "regex" : { "$regex" : "regexp", "$options" : "i" } }"
string(55) "{ "regex" : { "$regex" : "regexp", "$options" : "i" } }"
bool(true)

Warning: MongoDB\BSON\Regex::getPattern() expects exactly 0 parameters, 1 given in %s on line %d

Warning: MongoDB\BSON\Regex::getFlags() expects exactly 0 parameters, 1 given in %s on line %d
===DONE===
