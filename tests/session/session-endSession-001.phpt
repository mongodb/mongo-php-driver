--TEST--
MongoDB\Driver\Session::endSession() Calling methods after session has been ended
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_no_transactions(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$sessionA = $manager->startSession();
$sessionA->endSession();

echo throws(function() use ($sessionA) {
    $sessionA->startTransaction();
}, 'MongoDB\Driver\Exception\LogicException'), "\n";

echo throws(function() use ($sessionA) {
    $sessionA->abortTransaction();
}, 'MongoDB\Driver\Exception\LogicException'), "\n";

/* The reason that startTransaction is in here twice is that this script can run without exception
 * if the endSession() call is taken out. */
echo throws(function() use ($sessionA) {
    $sessionA->startTransaction();
}, 'MongoDB\Driver\Exception\LogicException'), "\n";

echo throws(function() use ($sessionA) {
    $sessionA->commitTransaction();
}, 'MongoDB\Driver\Exception\LogicException'), "\n";

echo throws(function() use ($sessionA) {
    $sessionA->advanceOperationTime(new \MongoDB\BSON\Timestamp(1900123000, 1900123000));
}, 'MongoDB\Driver\Exception\LogicException'), "\n";

echo throws(function() use ($sessionA) {
    $sessionA->advanceClusterTime([]);
}, 'MongoDB\Driver\Exception\LogicException'), "\n";

echo throws(function() use ($sessionA) {
    var_dump($sessionA->getClusterTime());
}, 'MongoDB\Driver\Exception\LogicException'), "\n";

echo throws(function() use ($sessionA) {
    var_dump($sessionA->getLogicalSessionId());
}, 'MongoDB\Driver\Exception\LogicException'), "\n";

echo throws(function() use ($sessionA) {
    var_dump($sessionA->getOperationTime());
}, 'MongoDB\Driver\Exception\LogicException'), "\n";

echo throws(function() use ($sessionA) {
    $sessionA->isInTransaction();
}, 'MongoDB\Driver\Exception\LogicException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\LogicException
Cannot call 'startTransaction', as the session has already been ended.
OK: Got MongoDB\Driver\Exception\LogicException
Cannot call 'abortTransaction', as the session has already been ended.
OK: Got MongoDB\Driver\Exception\LogicException
Cannot call 'startTransaction', as the session has already been ended.
OK: Got MongoDB\Driver\Exception\LogicException
Cannot call 'commitTransaction', as the session has already been ended.
OK: Got MongoDB\Driver\Exception\LogicException
Cannot call 'advanceOperationTime', as the session has already been ended.
OK: Got MongoDB\Driver\Exception\LogicException
Cannot call 'advanceClusterTime', as the session has already been ended.
OK: Got MongoDB\Driver\Exception\LogicException
Cannot call 'getClusterTime', as the session has already been ended.
OK: Got MongoDB\Driver\Exception\LogicException
Cannot call 'getLogicalSessionId', as the session has already been ended.
OK: Got MongoDB\Driver\Exception\LogicException
Cannot call 'getOperationTime', as the session has already been ended.
OK: Got MongoDB\Driver\Exception\LogicException
Cannot call 'isInTransaction', as the session has already been ended.
===DONE===
