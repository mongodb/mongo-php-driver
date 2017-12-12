--TEST--
MongoDB\Driver\Manager::__construct(): invalid write concern (safe)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

// Invalid types

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://127.0.0.1/?safe=invalid');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager(null, ['safe' => 'invalid']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://127.0.0.1/?safe=invalid'. Unsupported value for "safe": "invalid".
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "safe" URI option, string given
===DONE===
