--TEST--
MongoDB\Driver\BulkWrite debug output after execution
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
    $bulk->insertOne(NS, ['foo' => 'bar']);
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
  object(MongoDB\Driver\Session)#2 (10) {
    %a
  }
}
===DONE===
