--TEST--
MongoDB\Driver\BulkWrite debug output after execution
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
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
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(['foo' => 'bar']);
    $manager->executeBulkWrite(NS, $bulk, $options);
    var_dump($bulk);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\BulkWrite)#%d (%d) {
  ["database"]=>
  %s
  ["collection"]=>
  %s
  ["ordered"]=>
  bool(true)
  ["bypassDocumentValidation"]=>
  NULL
  ["executed"]=>
  bool(true)
  ["server_id"]=>
  int(%d)
  ["session"]=>
  NULL
  ["write_concern"]=>
  NULL
}
object(MongoDB\Driver\BulkWrite)#%d (%d) {
  ["database"]=>
  %s
  ["collection"]=>
  %s
  ["ordered"]=>
  bool(true)
  ["bypassDocumentValidation"]=>
  NULL
  ["executed"]=>
  bool(true)
  ["server_id"]=>
  int(%d)
  ["session"]=>
  object(MongoDB\Driver\Session)#%d (%d) {
    %a
  }
  ["write_concern"]=>
  NULL
}
===DONE===
