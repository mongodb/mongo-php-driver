--TEST--
PHPC-1598: BSON type get_gc should delegate to zend_std_get_properties
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '8.1.99'); ?>
--FILE--
<?php

$tests = [
    [ 'binary' => new MongoDB\BSON\Binary('foo', MongoDB\BSON\Binary::TYPE_GENERIC) ],
    // Use serialized strings to construct types with disabled constructors
    [ 'dbpointer' => unserialize('C:22:"MongoDB\BSON\DBPointer":76:{a:2:{s:3:"ref";s:11:"phongo.test";s:2:"id";s:24:"5a2e78accd485d55b4050000";}}') ],
    [ 'decimal128' => new MongoDB\BSON\Decimal128('1234.5678') ],
    [ 'int64' => new MongoDB\BSON\Int64('9223372036854775807') ],
    // JavaScript w/ scope may not be necessary (same code path as w/o scope), but we'll test it anyway
    [ 'javascript' => new MongoDB\BSON\Javascript('function() { return 1; }') ],
    [ 'javascript_ws' => new MongoDB\BSON\Javascript('function() { return a; }', ['a' => 1]) ],
    // MaxKey and MinKey don't have get_properties or get_gc handlers, but we'll test them anyway
    [ 'maxkey' => new MongoDB\BSON\MaxKey ],
    [ 'minkey' => new MongoDB\BSON\MinKey ],
    [ 'objectid' => new MongoDB\BSON\ObjectId ],
    [ 'regex' => new MongoDB\BSON\Regex('pattern', 'i') ],
    [ 'symbol' => unserialize('C:19:"MongoDB\BSON\Symbol":38:{a:1:{s:6:"symbol";s:11:"symbolValue";}}') ],
    [ 'timestamp' => new MongoDB\BSON\Timestamp(1234, 5678) ],
    [ 'utcdatetime' => new MongoDB\BSON\UTCDateTime ],
];

foreach ($tests as &$test) {
    echo key($test), "\n";

    /* Store an additional object reference as a public property on the BSON
     * object. This will leak if get_gc returns internally cached properties
     * (from our get_properties handler) instead of zend_std_get_properties. */
    $a = new stdClass;
    $a->bson = current($test);
    $a->bson->a = $a;

    /* Unset the original BSON object from the test so that its only reference
     * is from the new object we just created. This also requires iterating the
     * test cases by reference. */
    unset($test[key($test)]);

    printf("Collected cycles: %d\n", gc_collect_cycles());

    unset($a);

    printf("Collected cycles: %d\n\n", gc_collect_cycles());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
binary
Collected cycles: 0
Collected cycles: 2

dbpointer
Collected cycles: 0
Collected cycles: 2

decimal128
Collected cycles: 0
Collected cycles: 2

int64
Collected cycles: 0
Collected cycles: 2

javascript
Collected cycles: 0
Collected cycles: 2

javascript_ws
Collected cycles: 0
Collected cycles: 2

maxkey
Collected cycles: 0
Collected cycles: 2

minkey
Collected cycles: 0
Collected cycles: 2

objectid
Collected cycles: 0
Collected cycles: 2

regex
Collected cycles: 0
Collected cycles: 2

symbol
Collected cycles: 0
Collected cycles: 2

timestamp
Collected cycles: 0
Collected cycles: 2

utcdatetime
Collected cycles: 0
Collected cycles: 2

===DONE===
