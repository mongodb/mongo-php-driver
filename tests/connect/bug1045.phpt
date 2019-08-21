--TEST--
PHPC-1045: Segfault if username is not provided for SCRAM-SHA-1 authMechanism
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

echo throws(function() {
    // URI may or may not support auth, but that is not necessary for the test
    new MongoDB\Driver\Manager('mongodb://127.0.0.1/', ['authMechanism' => 'SCRAM-SHA-1']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: 'SCRAM-SHA-1' authentication mechanism requires username.
===DONE===
