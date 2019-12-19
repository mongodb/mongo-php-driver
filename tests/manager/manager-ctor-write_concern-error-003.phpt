--TEST--
MongoDB\Driver\Manager::__construct(): invalid write concern (wtimeoutms)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://127.0.0.1/?wtimeoutms=invalid');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager(null, ['wTimeoutMS' => 'invalid']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";


?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://127.0.0.1/?wtimeoutms=invalid'. Unsupported value for "wtimeoutms": "invalid".
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected integer for "wTimeoutMS" URI option, string given
===DONE===
