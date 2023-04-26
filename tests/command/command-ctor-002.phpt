--TEST--
MongoDB\Driver\Command::__construct() $document is MongoDB\Driver\Document
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$document = MongoDB\BSON\Document::fromJSON('{ "ping": 1 }');
$command = new MongoDB\Driver\Command($document);

var_dump($command);

$cursor = $manager->executeCommand(DATABASE_NAME, $command);

var_dump($cursor->toArray()[0]);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\Command)#%d (%d) {
  ["command"]=>
  object(stdClass)#%d (%d) {
    ["ping"]=>
    int(1)
  }
}
object(stdClass)#%d (%d) {
  ["ok"]=>
  float(1)%A
}
===DONE===
