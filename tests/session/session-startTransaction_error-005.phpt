--TEST--
MongoDB\Driver\Session::startTransaction() with wrong argument for options array (PHP 8)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto() ?>
<?php skip_if_no_transactions(); ?>
<?php skip_if_php_version('<', '7.99'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

/* Note: PHP 8 throws a TypeError during zpp (vs. arginfo in PHP 7) */

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
%r\\?%rMongoDB\Driver\Session::startTransaction()%sarray, int given
OK: Got TypeError
%r\\?%rMongoDB\Driver\Session::startTransaction()%sarray, %r(object|stdClass)%r given
===DONE===
