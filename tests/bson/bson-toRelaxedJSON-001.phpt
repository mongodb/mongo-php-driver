--TEST--
MongoDB\BSON\toRelaxedExtendedJSON(): Encoding JSON
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    [],
    [ 'null' => null ],
    [ 'boolean' => true ],
    [ 'string' => 'foo' ],
    [ 'integer' => 123 ],
    [ 'double' => 1.0, ],
    [ 'nan' => NAN ],
    [ 'pos_inf' => INF ],
    [ 'neg_inf' => -INF ],
    [ 'array' => [ 'foo', 'bar' ]],
    [ 'document' => [ 'foo' => 'bar' ]],
];

foreach ($tests as $value) {
    $bson = fromPHP($value);
    echo toRelaxedExtendedJSON($bson), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
{ }
{ "null" : null }
{ "boolean" : true }
{ "string" : "foo" }
{ "integer" : 123 }
{ "double" : 1.0 }
{ "nan" : { "$numberDouble" : "NaN" } }
{ "pos_inf" : { "$numberDouble" : "Infinity" } }
{ "neg_inf" : { "$numberDouble" : "-Infinity" } }
{ "array" : [ "foo", "bar" ] }
{ "document" : { "foo" : "bar" } }
===DONE===
