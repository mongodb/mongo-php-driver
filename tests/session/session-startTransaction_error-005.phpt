--TEST--
MongoDB\Driver\Session::startTransaction() with wrong argument for options array on PHP 7
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto() ?>
<?php skip_if_no_transactions(); ?>
<?php skip_if_php_version('<', '7.1.0'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
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
Argument 1 passed to MongoDB\Driver\Session::startTransaction() must be of the type array or null, int%S given
OK: Got TypeError
Argument 1 passed to MongoDB\Driver\Session::startTransaction() must be of the type array or null, object given
===DONE===
