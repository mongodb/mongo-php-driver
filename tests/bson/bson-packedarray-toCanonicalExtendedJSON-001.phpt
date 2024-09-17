--TEST--
MongoDB\BSON\PackedArray::toCanonicalExtendedJSON(): Encoding JSON
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
    echo MongoDB\BSON\PackedArray::fromPHP($value)->toCanonicalExtendedJSON(), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
[ ]
[ null ]
[ true ]
[ "foo" ]
[ { "$numberInt" : "123" } ]
[ { "$numberDouble" : "1.0" } ]
[ { "$numberDouble" : "NaN" } ]
[ { "$numberDouble" : "Infinity" } ]
[ { "$numberDouble" : "-Infinity" } ]
[ [ "foo", "bar" ] ]
[ { "foo" : "bar" } ]
===DONE===
