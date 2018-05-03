--TEST--
MongoDB\Driver\Manager::executeCommand()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$command = new MongoDB\Driver\Command(array('ping' => 1));
$result = $manager->executeCommand(DATABASE_NAME, $command);
var_dump($command);

var_dump($result instanceof MongoDB\Driver\Cursor);
var_dump($result);

echo "\nDumping response document:\n";
var_dump(current($result->toArray()));

$server = $result->getServer();

var_dump($server instanceof MongoDB\Driver\Server);
var_dump($server->getHost());
var_dump($server->getPort());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\Command)#%d (%d) {
  ["command"]=>
  object(stdClass)#%d (1) {
    ["ping"]=>
    int(1)
  }
}
bool(true)
object(MongoDB\Driver\Cursor)#%d (%d) {
  ["database"]=>
  string(6) "phongo"
  ["collection"]=>
  NULL
  ["query"]=>
  NULL
  ["command"]=>
  object(MongoDB\Driver\Command)#%d (%d) {
    ["command"]=>
    object(stdClass)#%d (%d) {
      ["ping"]=>
      int(1)
    }
  }
  ["readPreference"]=>
  NULL
  ["session"]=>
  %a
  ["isDead"]=>
  bool(false)
  ["currentIndex"]=>
  int(0)
  ["currentDocument"]=>
  NULL
  ["server"]=>
  object(MongoDB\Driver\Server)#%d (%d) {
    %a
  }
}

Dumping response document:
object(stdClass)#%d (%d) {
  ["ok"]=>
  float(1)%A
}
bool(true)
string(%d) "%s"
int(%d)
===DONE===
