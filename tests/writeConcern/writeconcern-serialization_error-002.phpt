--TEST--
MongoDB\Driver\WriteConcern unserialization errors (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* Note: this file does not exhaustively test all code paths in
 * php_phongo_readpreference_init_from_hash since those are already covered by
 * tests for ReadPreference::__set_state() */

echo throws(function() {
    unserialize('O:27:"MongoDB\Driver\WriteConcern":2:{s:1:"w";i:0;s:1:"j";b:1;}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot enable journaling when using w = 0
===DONE===
