--TEST--
MongoDB\Driver\WriteConcernError::getInfo() exposes writeConcernError.errInfo
--DESCRIPTION--
CRUD spec prose test #1
https://github.com/mongodb/specifications/blob/master/source/crud/tests/README.rst#writeconcernerror-details-exposes-writeconcernerror-errinfo
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_no_failcommand_failpoint(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

// Select a specific server for future operations to avoid mongos switching in sharded clusters
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference('primary'));

configureTargetedFailPoint(
    $server,
    'failCommand',
    [ 'times' => 1],
    [
        'failCommands' => ['insert'],
        'writeConcernError' => [
            'code' => 100,
            'codeName' => 'UnsatisfiableWriteConcern',
            'errmsg' => 'Not enough data-bearing nodes',
            'errInfo' => [
                'writeConcern' => [
                    'w' => 2,
                    'wtimeout' => 0,
                    'provenance' => 'clientSupplied',
                ],
            ],
        ],
    ]
);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['_id' => 1]);

try {
    $server->executeBulkWrite(NS, $bulk);
} catch (MongoDB\Driver\Exception\BulkWriteException $e) {
    var_dump($e->getWriteResult()->getWriteConcernError()->getInfo());
}

?>
===DONE===
--EXPECTF--
object(stdClass)#%d (%d) {
  ["writeConcern"]=>
  object(stdClass)#%d (%d) {
    ["w"]=>
    int(2)
    ["wtimeout"]=>
    int(0)
    ["provenance"]=>
    string(14) "clientSupplied"
  }
}
===DONE===
