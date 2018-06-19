--TEST--
MongoDB\Driver\Session::startTransaction() with wrong argument for options array
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_server_version('<', '4.0'); ?>
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
    $session->startTransaction($txnOptions);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Warning: MongoDB\Driver\Session::startTransaction() expects parameter 1 to be array, integer given in %s on line %d

Warning: MongoDB\Driver\Session::startTransaction() expects parameter 1 to be array, object given in %s on line %d
===DONE===
