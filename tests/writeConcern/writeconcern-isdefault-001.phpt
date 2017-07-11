--TEST--
MongoDB\Driver\WriteConcern::isDefault()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$tests = array(
    new MongoDB\Driver\WriteConcern(1),
    (new MongoDB\Driver\Manager('mongodb://127.0.0.1/?w=1'))->getWriteConcern(),
    (new MongoDB\Driver\Manager(null, ['w' => 1]))->getWriteConcern(),
    (new MongoDB\Driver\Manager)->getWriteConcern(),
);

foreach ($tests as $wc) {
    var_dump($wc->isDefault());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(false)
bool(false)
bool(false)
bool(true)
===DONE===
