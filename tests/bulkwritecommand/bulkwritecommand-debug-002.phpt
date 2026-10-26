--TEST--
MongoDB\Driver\BulkWriteCommand debug output after execution
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '8.0'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$tests = [
    [],
    ['session' => $manager->startSession()],
];

foreach ($tests as $options) {
    $bulk = new MongoDB\Driver\BulkWriteCommand();
    $bulk->insertOne(NS, ['x' => 1]);
    $manager->executeBulkWriteCommand($bulk, $options);
    var_dump($bulk);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\BulkWriteCommand)#%d (%d) {
  ["bypassDocumentValidation"]=>
  NULL
  ["ordered"]=>
  bool(true)
  ["verboseResults"]=>
  bool(false)
  ["session"]=>
  NULL
}
object(MongoDB\Driver\BulkWriteCommand)#%d (%d) {
  ["bypassDocumentValidation"]=>
  NULL
  ["ordered"]=>
  bool(true)
  ["verboseResults"]=>
  bool(false)
  ["session"]=>
  object(MongoDB\Driver\Session)#%d (%d) {
    %a
  }
}
===DONE===
