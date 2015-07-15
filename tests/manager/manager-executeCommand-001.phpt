--TEST--
MongoDB\Driver\Manager::executeCommand()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

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
object(MongoDB\Driver\Command)#%d (1) {
  ["command"]=>
  object(stdClass)#%d (1) {
    ["ping"]=>
    int(1)
  }
}
bool(true)
object(MongoDB\Driver\Cursor)#%d (%d) {
  ["cursor"]=>
  array(19) {
    ["stamp"]=>
    int(0)
    ["is_command"]=>
    bool(true)
    ["sent"]=>
    bool(true)
    ["done"]=>
    bool(false)
    ["failed"]=>
    bool(false)
    ["end_of_event"]=>
    bool(false)
    ["in_exhaust"]=>
    bool(false)
    ["redir_primary"]=>
    bool(false)
    ["has_fields"]=>
    bool(false)
    ["query"]=>
    object(stdClass)#%d (1) {
      ["ping"]=>
      int(1)
    }
    ["fields"]=>
    object(stdClass)#%d (0) {
    }
    ["read_preference"]=>
    array(2) {
      ["mode"]=>
      int(1)
      ["tags"]=>
      array(0) {
      }
    }
    ["flags"]=>
    int(0)
    ["skip"]=>
    int(0)
    ["limit"]=>
    int(1)
    ["count"]=>
    int(1)
    ["batch_size"]=>
    int(0)
    ["ns"]=>
    string(11) "phongo.$cmd"
    ["current_doc"]=>
    object(stdClass)#%d (1) {
      ["ok"]=>
      float(1)
    }
  }
  ["server_id"]=>
  int(1)
}

Dumping response document:
object(stdClass)#%d (1) {
  ["ok"]=>
  float(1)
}
bool(true)
string(%d) "%s"
int(%d)
===DONE===
