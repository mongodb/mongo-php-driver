--TEST--
MongoDB\Driver\Session::startTransaction() with wrong argument for options array (PHP 7)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto() ?>
<?php skip_if_no_transactions(); ?>
<?php skip_if_php_version('>', '7.99'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

/* Note: PHP 7 throws a non-array TypeError based on arginfo, which happens
 * before zpp and our proxying of errors with InvalidArgumentExceptions. */

$manager = create_test_manager();
$session = $manager->startSession();

$options = [
    2,
    new stdClass,
];

foreach ($options as $txnOptions) {
    echo throws(function () use ($session, $txnOptions) {
        $session->startTransaction($txnOptions);
    }, TypeError::class), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got TypeError
Argument 1 passed to MongoDB\Driver\Session::startTransaction() must be of the type array%r( or null)?%r, int%S given
OK: Got TypeError
Argument 1 passed to MongoDB\Driver\Session::startTransaction() must be of the type array%r( or null)?%r, object given
===DONE===
