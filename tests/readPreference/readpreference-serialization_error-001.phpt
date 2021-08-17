--TEST--
MongoDB\Driver\ReadPreference unserialization errors (Serializable interface)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* Note: this file does not exhaustively test all code paths in
 * php_phongo_readpreference_init_from_hash since those are already covered by
 * tests for ReadPreference::__set_state() */

echo throws(function() {
    unserialize('C:29:"MongoDB\Driver\ReadPreference":31:{a:1:{s:4:"mode";s:7:"invalid";}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('C:29:"MongoDB\Driver\ReadPreference":21:{a:1:{s:4:"mode";i:0;}}');
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
