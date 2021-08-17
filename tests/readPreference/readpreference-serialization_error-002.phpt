--TEST--
MongoDB\Driver\ReadPreference unserialization errors (__serialize and __unserialize)
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
    unserialize('O:29:"MongoDB\Driver\ReadPreference":1:{s:4:"mode";s:7:"invalid";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('O:29:"MongoDB\Driver\ReadPreference":1:{s:4:"mode";i:0;}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ReadPreference initialization requires specific values for "mode" string field
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ReadPreference initialization requires "mode" field to be string
===DONE===
