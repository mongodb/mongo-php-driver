--TEST--
MongoDB\Driver\Manager::__construct(): invalid read preference
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(STANDALONE . '/?readPreference=primary&readPreferenceTags=dc:ny');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(STANDALONE, array('readPreference' => 'nothing'));
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(STANDALONE . '/?readPreference=primary', array('readPreferenceTags' => array(array())));
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: '%s/?readPreference=primary&readPreferenceTags=dc:ny'
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Unsupported readPreference value: 'nothing'
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Primary read preference mode conflicts with tags
===DONE===
