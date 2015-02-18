--TEST--
MongoDB\Driver\Manager::executeQuery() one document
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Driver\Manager(MONGODB_URI);

// load fixtures for test
$batch = new MongoDB\Driver\WriteBatch();
$batch->insert(array('_id' => 1, 'x' => 2, 'y' => 3));
$batch->insert(array('_id' => 2, 'x' => 3, 'y' => 4));
$batch->insert(array('_id' => 3, 'x' => 4, 'y' => 5));
$manager->executeWriteBatch(NS, $batch);

$query = new MongoDB\Driver\Query(array('x' => 3), array('projection' => array('y' => 1)));
$qr = $manager->executeQuery(NS, $query);

var_dump($qr instanceof MongoDB\Driver\Result);
var_dump($qr);

$server = $qr->getServer();

var_dump($server instanceof MongoDB\Driver\Server);
var_dump($server->getHost());
var_dump($server->getPort());

var_dump(iterator_to_array($qr));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
object(MongoDB\Driver\Result)#%d (%d) {
  ["cursor"]=>
  array(19) {
    ["stamp"]=>
    int(0)
    ["is_command"]=>
    bool(false)
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
    bool(true)
    ["query"]=>
    array(1) {
      ["$query"]=>
      object(stdClass)#%d (%d) {
        ["x"]=>
        int(3)
      }
    }
    ["fields"]=>
    array(1) {
      ["y"]=>
      int(1)
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
    int(0)
    ["count"]=>
    int(1)
    ["batch_size"]=>
    int(0)
    ["ns"]=>
    string(42) "phongo.standalone_manager_executeQuery_001"
    ["current_doc"]=>
    array(2) {
      ["_id"]=>
      int(2)
      ["y"]=>
      int(4)
    }
  }
  ["firstBatch"]=>
  array(2) {
    ["_id"]=>
    int(2)
    ["y"]=>
    int(4)
  }
  ["hint"]=>
  int(1)
  ["is_command_cursor"]=>
  bool(false)
}
bool(true)
string(%d) "%s"
int(27017)
array(1) {
  [0]=>
  array(2) {
    ["_id"]=>
    int(2)
    ["y"]=>
    int(4)
  }
}
===DONE===
