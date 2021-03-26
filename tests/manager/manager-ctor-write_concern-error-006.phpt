--TEST--
MongoDB\Driver\Manager::__construct(): invalid write concern (safe)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

// Invalid types

echo throws(function() {
    create_test_manager('mongodb://127.0.0.1/?safe=invalid');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager(null, ['safe' => 'invalid']);
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
