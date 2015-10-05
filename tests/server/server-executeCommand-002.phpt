--TEST--
MongoDB\Driver\Server::executeCommand() takes a read preference
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; NEEDS("REPLICASET"); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(REPLICASET);

$rp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY);
$secondary = $manager->selectServer($rp);

$command = new MongoDB\Driver\Command(array('profile' => 2));
$cursor = $secondary->executeCommand(DATABASE_NAME, $command);
$result = current($cursor->toArray());

printf("Set profile level to 2 successfully: %s\n", (empty($result->ok) ? 'no' : 'yes'));

$command = new MongoDB\Driver\Command(array(
    'aggregate' => COLLECTION_NAME,
    'pipeline' => array(array('$match' => array('x' => 1))),
));
$secondary->executeCommand(DATABASE_NAME, $command, $rp);

$query = new MongoDB\Driver\Query(
    array(
        'op' => 'command',
        'ns' => DATABASE_NAME . '.$cmd',
    ),
    array(
        'sort' => array('ts' => -1),
        'limit' => 1,
    )
);
$cursor = $secondary->executeQuery(DATABASE_NAME . '.system.profile', $query, $rp);
$profileEntry = current($cursor->toArray());

var_dump($profileEntry->command);

$command = new MongoDB\Driver\Command(array('profile' => 0));
$cursor = $secondary->executeCommand(DATABASE_NAME, $command);
$result = current($cursor->toArray());

printf("Set profile level to 0 successfully: %s\n", (empty($result->ok) ? 'no' : 'yes'));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Set profile level to 2 successfully: yes
object(stdClass)#%d (%d) {
  ["aggregate"]=>
  string(32) "server_server_executeCommand_002"
  ["pipeline"]=>
  array(1) {
    [0]=>
    object(stdClass)#%d (%d) {
      ["$match"]=>
      object(stdClass)#%d (%d) {
        ["x"]=>
        int(1)
      }
    }
  }
}
Set profile level to 0 successfully: yes
===DONE===
