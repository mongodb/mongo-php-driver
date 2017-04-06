--TEST--
MongoDB\BSON\toExtendedJSON(): Encoding JSON
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    [],
    [ 'foo' => 'bar' ],
    [ 'foo' => [ 1, 2, 3 ]],
    [ 'foo' => [ 'bar' => 1 ]],
];

foreach ($tests as $value) {
    $bson = fromPHP($value);
    echo toExtendedJSON($bson), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
{ }
{ "foo" : "bar" }
{ "foo" : [ { "$numberInt" : "1" }, { "$numberInt" : "2" }, { "$numberInt" : "3" } ] }
{ "foo" : { "bar" : { "$numberInt" : "1" } } }
===DONE===
