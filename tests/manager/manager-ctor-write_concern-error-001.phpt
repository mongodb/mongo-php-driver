--TEST--
MongoDB\Driver\Manager::__construct(): invalid write concern (w)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* Note: libmongoc doesn't check w's type in the URI string. Numeric strings are
 * parsed as 32-bit integers with strtol() and other values are used as-is. */

echo throws(function() {
    create_test_manager(null, ['w' => 1.0]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager(null, ['w' => -1]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer or string for "w" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Unsupported w value: -1
===DONE===
