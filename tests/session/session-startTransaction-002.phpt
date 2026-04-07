--TEST--
MongoDB\Driver\Session::startTransaction() with all valid options combined
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_no_transactions(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$session = $manager->startSession();

$session->startTransaction([
    'maxCommitTimeMS' => 5000,
    'readConcern' => new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::MAJORITY),
    'readPreference' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY),
    'writeConcern' => new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY),
]);

$options = $session->getTransactionOptions();

var_dump(isset($options['maxCommitTimeMS']));
var_dump($options['maxCommitTimeMS']);
var_dump($options['readConcern']->getLevel());
var_dump($options['readPreference']->getModeString());
var_dump($options['writeConcern']->getW());

$session->abortTransaction();

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
int(5000)
string(8) "majority"
string(7) "primary"
string(8) "majority"
===DONE===
