--TEST--
PHPC-671: Segfault if Manager is already freed when using WriteResult's Server
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['_id' => 1]);
$writeResult = $manager->executeBulkWrite(NS, $bulk);

unset($manager);

$server = $writeResult->getServer();

/* WriteResult only uses the client to construct a Server. We need to interact
 * with the Server to test for a user-after-free. */
$cursor = $server->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));
var_dump($cursor->toArray()[0]);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["ok"]=>
  float(1)%A
}
===DONE===
