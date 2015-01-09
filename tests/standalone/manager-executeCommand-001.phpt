--TEST--
MongoDB\Driver\Manager::executeCommand()
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Driver\Manager(MONGODB_URI);

$command = new MongoDB\Driver\Command(array('ping' => 1));
$result = $manager->executeCommand(DATABASE_NAME, $command);
var_dump($command);

var_dump($result instanceof MongoDB\Driver\CommandResult);

echo "Dumping response document:\n";
var_dump($result->getResponseDocument());

echo "Dumping iterated result:\n";
var_dump(iterator_to_array($result));

$server = $result->getServer();

var_dump($server instanceof MongoDB\Driver\Server);
var_dump($server->getHost());
var_dump($server->getPort());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\Command)#%d (1) {
  ["command"]=>
  array(1) {
    ["ping"]=>
    int(1)
  }
}
bool(true)
Dumping response document:
array(1) {
  ["ok"]=>
  float(1)
}
Dumping iterated result:
array(1) {
  [0]=>
  array(1) {
    ["ok"]=>
    float(1)
  }
}
bool(true)
string(%d) "%s"
int(%d)
===DONE===
