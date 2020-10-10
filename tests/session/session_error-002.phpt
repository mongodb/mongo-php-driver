--TEST--
MongoDB\Driver\Session with wrong defaultTransactionOptions
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '7.99'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager();

echo raises(function() use ($manager) {
    $manager->startSession([
        'defaultTransactionOptions' => [ 'maxCommitTimeMS' => new stdClass ]
    ]);
}, E_NOTICE), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got E_NOTICE
Object of class stdClass could not be converted to int
===DONE===
