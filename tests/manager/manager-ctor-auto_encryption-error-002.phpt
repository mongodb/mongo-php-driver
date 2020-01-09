--TEST--
MongoDB\Driver\Manager::__construct(): auto encryption when compiling without libmongocrypt
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_libmongocrypt(); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function () {
    $manager = new MongoDB\Driver\Manager(null, [], ['autoEncryption' => []]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot enable automatic field-level encryption. Please recompile with support for libmongocrypt using the with-mongodb-client-side-encryption configure switch.
===DONE===
