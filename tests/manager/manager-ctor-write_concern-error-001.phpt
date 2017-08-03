--TEST--
MongoDB\Driver\Manager::__construct(): invalid write concern (w)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

/* Note: libmongoc doesn't check w's type in the URI string. Numeric strings are
 * parsed as 32-bit integers with strtol() and other values are used as-is. */

echo throws(function() {
    new MongoDB\Driver\Manager(null, ['w' => 1.0]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

/* Note: Values of w < -1 are invalid, but libmongoc's URI string parsing only
 * logs a warning instead of raising an error (see: CDRIVER-2234), so we cannot
 * test for this. */

echo throws(function() {
    new MongoDB\Driver\Manager(null, ['w' => -2]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer or string for "w" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Unsupported w value: -2
===DONE===
