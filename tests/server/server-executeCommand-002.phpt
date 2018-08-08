--TEST--
MongoDB\Driver\Server::executeCommand() takes a read preference
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_no_secondary(); ?>
<?php skip_if_server_version('<', '3.2'); ?>
<?php skip_if_not_clean(DATABASE_NAME, 'system.profile'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$rp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY);
$secondary = $manager->selectServer($rp);

$command = new MongoDB\Driver\Command(array('profile' => 2));
$cursor = $secondary->executeCommand(DATABASE_NAME, $command);
$result = current($cursor->toArray());

printf("Set profile level to 2 successfully: %s\n", (empty($result->ok) ? 'no' : 'yes'));

$command = new MongoDB\Driver\Command([
    'aggregate' => COLLECTION_NAME,
    'pipeline' => [ [ '$match' => [ 'x' => 1 ] ] ],
    'cursor' => (object) [],
]);
$secondary->executeCommand(DATABASE_NAME, $command, $rp);

$query = new MongoDB\Driver\Query(
    array(
        'op' => 'command',
        'ns' => DATABASE_NAME . '.' . COLLECTION_NAME,
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
  ["cursor"]=>
  object(stdClass)#%d (%d) {
  }%A
}
Set profile level to 0 successfully: yes
===DONE===
