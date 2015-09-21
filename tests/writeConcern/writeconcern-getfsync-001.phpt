--TEST--
MongoDB\Driver\WriteConcern::getFsync()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$tests = array(
    true,
    false,
    1,
    0,
    null,
);

foreach ($tests as $test) {
    $wc = new MongoDB\Driver\WriteConcern(1, 0, true, $test);
    var_dump($wc->getFsync());
}

// Test with default value
$wc = new MongoDB\Driver\WriteConcern(1, 0, true);
var_dump($wc->getFsync());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(false)
bool(true)
bool(false)
NULL
NULL
===DONE===
