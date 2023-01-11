--TEST--
MongoDB\Driver\WriteResult::getWriteConcernError() works for errors without a message
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_mongos(); ?>
<?php skip_if_no_failcommand_failpoint(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager(null, ['retryWrites' => false]);

// Configure a fail point triggering a write concern error without message
configureFailPoint(
    $manager,
    'failCommand',
    ['times' => 1],
    [
        'failCommands' => ['insert'],
        'writeConcernError' => ['code' => 91],
    ]
);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['write' => 1]);
$result = $manager->executeBulkWrite(NS, $bulk);

var_dump($result->getWriteConcernError());

configureFailPoint($manager, 'failCommand', 'off');

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\WriteConcernError)#%d (%d) {
  ["message"]=>
  string(0) ""
  ["code"]=>
  int(91)
  ["info"]=>
  NULL
}
===DONE===
