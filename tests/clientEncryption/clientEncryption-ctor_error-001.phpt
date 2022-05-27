--TEST--
MongoDB\Driver\ClientEncryption::__construct() fails if compiled without FLE
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_libmongocrypt(); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function () {
    new MongoDB\Driver\ClientEncryption([]);
}, MongoDB\Driver\Exception\RuntimeException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\RuntimeException
Cannot configure clientEncryption object. Please recompile with support for libmongocrypt using the with-mongodb-client-side-encryption configure switch.
===DONE===
