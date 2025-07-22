--TEST--
PHPC-2505: gc_collect_cycles() may interfere with using foreach to iterate objects
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

gc_disable();

$tests = [
    [ 'binary' => new MongoDB\BSON\Binary('foo', MongoDB\BSON\Binary::TYPE_GENERIC) ],
    [ 'dbpointer' => createDBPointer() ],
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
    [ 'symbol' => createSymbol() ],
    [ 'timestamp' => new MongoDB\BSON\Timestamp(1234, 5678) ],
    [ 'utcdatetime' => new MongoDB\BSON\UTCDateTime ],
];

ob_start();
foreach ($tests as $test) {
    echo key($test), "\n";
    $test = reset($test);

    foreach ($test as $k => $v) {
        var_dump($k, $v);
    }
}
$buf1 = ob_get_clean();
if ($buf1 === false) {
    throw new \AssertionError("Could not flush buffer");
}

gc_enable();
gc_collect_cycles();

ob_start();
foreach ($tests as $test) {
    echo key($test), "\n";
    $test = reset($test);

    foreach ($test as $k => $v) {
        var_dump($k, $v);
    }
}
$buf2 = ob_get_clean();
if ($buf2 === false) {
    throw new \AssertionError("Could not flush buffer");
}

if ($buf1 === $buf2) {
    echo "OK!\n";
    exit(0);
} else {
    echo("buf1 != buf2: $buf1\n\n IS NOT EQUAL TO \n\n$buf2\n");
    exit(1);
}

?>
--EXPECT--
OK!