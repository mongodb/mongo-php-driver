--TEST--
MongoDB\BSON\toJSON(): Encoding JSON
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
    /* Note: toJSON() does not properly handle NAN and INF values.
     * toCanonicalExtendedJSON() or toRelaxedExtendedJSON() should be used
     * instead. */
    [ 'nan' => NAN ],
    [ 'pos_inf' => INF ],
    [ 'neg_inf' => -INF ],
    [ 'array' => [ 'foo', 'bar' ]],
    [ 'document' => [ 'foo' => 'bar' ]],
];

foreach ($tests as $value) {
    $bson = fromPHP($value);
    echo toJSON($bson), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
{ }
{ "null" : null }
{ "boolean" : true }
{ "string" : "foo" }
{ "integer" : 123 }
{ "double" : 1.0 }
{ "nan" : %r-?nan(\(ind\))?%r }
{ "pos_inf" : inf }
{ "neg_inf" : -inf }
{ "array" : [ "foo", "bar" ] }
{ "document" : { "foo" : "bar" } }
===DONE===
