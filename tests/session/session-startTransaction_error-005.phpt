--TEST--
MongoDB\Driver\Session::startTransaction() with wrong argument for options array
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto() ?>
<?php skip_if_no_transactions(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";


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
