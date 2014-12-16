--TEST--
MongoDB\Manager::executeCommand()
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Manager(MONGODB_URI);

$command = new MongoDB\Command(array('ping' => 1));
$result = $manager->executeCommand(DATABASE_NAME, $command);

var_dump($result instanceof MongoDB\CommandResult);

echo "Dumping response document:\n";
var_dump($result->getResponseDocument());

echo "Dumping iterated result:\n";
var_dump(iterator_to_array($result));

$server = $result->getServer();

var_dump($server instanceof MongoDB\Server);
var_dump($server->getHost());
var_dump($server->getPort());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
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
