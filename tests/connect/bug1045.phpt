--TEST--
PHPC-1045: Segfault if username is not provided for SCRAM-SHA-1 authMechanism
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS_SSL(); ?>
<?php NEEDS('STANDALONE'); CLEANUP(STANDALONE); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

// STANDALONE does not support auth, but that is not necessary for the test
$m = new MongoDB\Driver\Manager(STANDALONE, ['authMechanism' => 'SCRAM-SHA-1', 'ssl' => false]);

// Execute a basic ping command to trigger connection initialization
echo throws(function() use ($m) {
    $m->executeCommand('admin', new MongoDB\Driver\Command(['ping'=>1]));
}, 'MongoDB\Driver\Exception\RuntimeException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\RuntimeException
SCRAM Failure: username is not set
===DONE===
