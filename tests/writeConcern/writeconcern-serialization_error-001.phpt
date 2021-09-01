--TEST--
MongoDB\Driver\WriteConcern unserialization errors (Serializable interface)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* Note: this file does not exhaustively test all code paths in
 * php_phongo_readpreference_init_from_hash since those are already covered by
 * tests for ReadPreference::__set_state() */

echo throws(function() {
    unserialize('C:27:"MongoDB\Driver\WriteConcern":30:{a:2:{s:1:"w";i:0;s:1:"j";b:1;}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot enable journaling when using w = 0
===DONE===
