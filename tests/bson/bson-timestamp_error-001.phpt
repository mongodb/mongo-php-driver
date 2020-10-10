--TEST--
MongoDB\BSON\Timestamp #001 error
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '7.99'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

throws(function() {
    new MongoDB\BSON\Timestamp;
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
===DONE===

