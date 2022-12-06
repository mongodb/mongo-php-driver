--TEST--
MongoDB\Driver\WriteResult::getErrorReplies()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
<?php skip_if_no_failcommand_failpoint(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$server = $manager->selectServer();

configureTargetedFailPoint(
    $server,
    'failCommand',
    ['times' => 1] ,
    ['errorCode' => 8, 'failCommands' => ['insert']]
);

$errors = [];
try {
    $bulk = new MongoDB\Driver\BulkWrite;
    $bulk->insert(['_id' => 1, 'x' => 'bar']);
    $server->executeBulkWrite(NS, $bulk);
} catch (MongoDB\Driver\Exception\BulkWriteException $e) {
    $errors = $e->getWriteResult()->getErrorReplies();
}

var_dump(count($errors));
var_dump($errors[0]->code);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
int(1)
int(8)
===DONE===
