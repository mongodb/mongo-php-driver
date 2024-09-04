--TEST--
MongoDB\BSON\toRelaxedExtendedJSON(): Encoding JSON
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

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
    $bson = MongoDB\BSON\fromPHP($value);
    echo MongoDB\BSON\toRelaxedExtendedJSON($bson), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toRelaxedExtendedJSON() is deprecated in %s
{ }

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toRelaxedExtendedJSON() is deprecated in %s
{ "null" : null }

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toRelaxedExtendedJSON() is deprecated in %s
{ "boolean" : true }

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toRelaxedExtendedJSON() is deprecated in %s
{ "string" : "foo" }

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toRelaxedExtendedJSON() is deprecated in %s
{ "integer" : 123 }

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toRelaxedExtendedJSON() is deprecated in %s
{ "double" : 1.0 }

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toRelaxedExtendedJSON() is deprecated in %s
{ "nan" : { "$numberDouble" : "NaN" } }

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toRelaxedExtendedJSON() is deprecated in %s
{ "pos_inf" : { "$numberDouble" : "Infinity" } }

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toRelaxedExtendedJSON() is deprecated in %s
{ "neg_inf" : { "$numberDouble" : "-Infinity" } }

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toRelaxedExtendedJSON() is deprecated in %s
{ "array" : [ "foo", "bar" ] }

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toRelaxedExtendedJSON() is deprecated in %s
{ "document" : { "foo" : "bar" } }
===DONE===
