--TEST--
MongoDB\Driver\Server::executeQuery() takes a read preference
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

if (empty($result->ok)) {
    exit("Could not set profile level\n");
}

$secondary->executeQuery(NS, new MongoDB\Driver\Query(array("x" => 1)), $rp);

$query = new MongoDB\Driver\Query(
    array(
        'op' => 'query',
        'ns' => NS,
    ),
    array(
        'sort' => array('ts' => -1),
        'limit' => 1,
    )
);
$cursor = $secondary->executeQuery(DATABASE_NAME . '.system.profile', $query, $rp);
$profileEntry = current($cursor->toArray());

var_dump($profileEntry->query);

$command = new MongoDB\Driver\Command(array('profile' => 0));
$cursor = $secondary->executeCommand(DATABASE_NAME, $command);
$result = current($cursor->toArray());

if (empty($result->ok)) {
    exit("Could not set profile level\n");
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["$query"]=>
  object(stdClass)#%d (%d) {
    ["x"]=>
    int(1)
  }
  ["$readPreference"]=>
  object(stdClass)#%d (%d) {
    ["mode"]=>
    string(9) "secondary"
    ["tags"]=>
    array(0) {
    }
  }
}
===DONE===
