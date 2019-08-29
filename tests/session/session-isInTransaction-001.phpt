--TEST--
MongoDB\Driver\Session::isInTransaction()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto() ?>
<?php skip_if_no_transactions(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

/* Create collections as that can't be (automatically) done in a transaction */
$cmd = new \MongoDB\Driver\Command([
    'create' => COLLECTION_NAME,
]);
$manager->executeCommand(DATABASE_NAME, $cmd);

/* Start a session */
$session = $manager->startSession();

/* Empty transaction, and aborted empty transaction */
var_dump($session->isInTransaction());
$session->startTransaction();
var_dump($session->isInTransaction());
$session->abortTransaction();
var_dump($session->isInTransaction());

/* Empty transaction, and committed empty transaction */
var_dump($session->isInTransaction());
$session->startTransaction();
var_dump($session->isInTransaction());
$session->commitTransaction();
var_dump($session->isInTransaction());

/* Aborted transaction with one operation */
var_dump($session->isInTransaction());
$session->startTransaction();
$bw = new \MongoDB\Driver\BulkWrite();
$bw->insert( [ '_id' => 0, 'msg' => 'Initial Value' ] );
$manager->executeBulkWrite(NS, $bw, ['session' => $session]);
var_dump($session->isInTransaction());
$session->abortTransaction();
var_dump($session->isInTransaction());

/* Committed transaction with one operation */
var_dump($session->isInTransaction());
$session->startTransaction();
$bw = new \MongoDB\Driver\BulkWrite();
$bw->insert( [ '_id' => 0, 'msg' => 'Initial Value' ] );
$manager->executeBulkWrite(NS, $bw, ['session' => $session]);
var_dump($session->isInTransaction());
$session->commitTransaction();
var_dump($session->isInTransaction());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(false)
bool(true)
bool(false)
bool(false)
bool(true)
bool(false)
bool(false)
bool(true)
bool(false)
bool(false)
bool(true)
bool(false)
===DONE===
