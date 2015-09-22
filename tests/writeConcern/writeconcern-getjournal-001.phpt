--TEST--
MongoDB\Driver\WriteConcern::getJournal()
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
    $wc = new MongoDB\Driver\WriteConcern(1, 0, $test);
    var_dump($wc->getJournal());
}

// Test with default value
$wc = new MongoDB\Driver\WriteConcern(1, 0);
var_dump($wc->getJournal());

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
