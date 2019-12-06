--TEST--
MongoDB\Driver\Session::getTransactionState()
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

echo "Test case: Empty transaction, and aborted empty transaction\n";
var_dump($session->getTransactionState());
$session->startTransaction();
var_dump($session->getTransactionState());
$session->abortTransaction();
var_dump($session->getTransactionState());
echo "\n";

echo "Test case: Empty transaction, and committed empty transaction\n";
$session->startTransaction();
var_dump($session->getTransactionState());
$session->commitTransaction();
var_dump($session->getTransactionState());
echo "\n";

echo "Test case: Aborted transaction with one operation\n";
$session->startTransaction();
var_dump($session->getTransactionState());
$bw = new \MongoDB\Driver\BulkWrite();
$bw->insert( [ '_id' => 0, 'msg' => 'Initial Value' ] );
$manager->executeBulkWrite(NS, $bw, ['session' => $session]);
var_dump($session->getTransactionState());
$session->abortTransaction();
var_dump($session->getTransactionState());
echo "\n";

echo "Test case: Committed transaction with one operation\n";
$session->startTransaction();
var_dump($session->getTransactionState());
$bw = new \MongoDB\Driver\BulkWrite();
$bw->insert( [ '_id' => 0, 'msg' => 'Initial Value' ] );
$manager->executeBulkWrite(NS, $bw, ['session' => $session]);
var_dump($session->getTransactionState());
$session->commitTransaction();
var_dump($session->getTransactionState());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Test case: Empty transaction, and aborted empty transaction
string(4) "none"
string(8) "starting"
string(7) "aborted"

Test case: Empty transaction, and committed empty transaction
string(8) "starting"
string(9) "committed"

Test case: Aborted transaction with one operation
string(8) "starting"
string(11) "in_progress"
string(7) "aborted"

Test case: Committed transaction with one operation
string(8) "starting"
string(11) "in_progress"
string(9) "committed"
===DONE===
