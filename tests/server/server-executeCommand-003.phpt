--TEST--
MongoDB\Driver\Server::executeCommand() with conflicting read preference for secondary
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_no_secondary(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$secondaryRp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY);
$secondary = $manager->selectServer($secondaryRp);

/* Note: this is testing that the read preference (even a conflicting one) has
 * no effect when directly querying a server, since the slaveOk flag is always
 * set for hinted commands. */
$primaryRp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY);
$cursor = $secondary->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(array('ping' => 1)), $primaryRp);

var_dump($cursor->toArray());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(1) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["ok"]=>
    float(1)%A
  }
}
===DONE===
