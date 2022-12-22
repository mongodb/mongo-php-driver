--TEST--
MongoDB\Driver\Manager::startSession() snapshot and causalConsistency cannot both be true
--DESCRIPTION--
Session spec prose test #1: Setting both snapshot and causalConsistency to true is not allowed
https://github.com/mongodb/specifications/blob/master/source/sessions/tests/README.rst#setting-both-snapshot-and-causalconsistency-to-true-is-not-allowed
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

echo throws(function() use ($manager) {
    $manager->startSession([
        'causalConsistency' => true,
        'snapshot' => true,
    ]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Only one of "causalConsistency" and "snapshot" can be enabled
===DONE===
