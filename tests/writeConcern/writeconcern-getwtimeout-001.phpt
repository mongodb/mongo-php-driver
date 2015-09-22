--TEST--
MongoDB\Driver\WriteConcern::getWtimeout()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$tests = array(
    0,
    1,
);

foreach ($tests as $test) {
    $wc = new MongoDB\Driver\WriteConcern(1, $test);
    var_dump($wc->getWtimeout());
}

// Test with default value
$wc = new MongoDB\Driver\WriteConcern(1);
var_dump($wc->getWtimeout());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
int(0)
int(1)
int(0)
===DONE===
