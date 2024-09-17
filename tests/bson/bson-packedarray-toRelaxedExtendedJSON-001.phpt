--TEST--
MongoDB\BSON\PackedArray::toRelaxedExtendedJSON(): Encoding JSON
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    [],
    [ null ],
    [ true ],
    [ 'foo' ],
    [ 123 ],
    [ 1.0, ],
    [ NAN ],
    [ INF ],
    [ -INF ],
    [ [ 'foo', 'bar' ]],
    [ [ 'foo' => 'bar' ]],
];

foreach ($tests as $value) {
    echo MongoDB\BSON\PackedArray::fromPHP($value)->toRelaxedExtendedJSON(), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
[ ]
[ null ]
[ true ]
[ "foo" ]
[ 123 ]
[ 1.0 ]
[ { "$numberDouble" : "NaN" } ]
[ { "$numberDouble" : "Infinity" } ]
[ { "$numberDouble" : "-Infinity" } ]
[ [ "foo", "bar" ] ]
[ { "foo" : "bar" } ]
===DONE===
