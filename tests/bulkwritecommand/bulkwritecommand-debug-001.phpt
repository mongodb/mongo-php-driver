--TEST--
MongoDB\Driver\BulkWriteCommand debug output
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$bulk = new MongoDB\Driver\BulkWriteCommand([
    'bypassDocumentValidation' => true,
    'comment' => 'foo',
    'let' => ['foo' => 1],
    'ordered' => true,
    'verboseResults' => true,
]);

var_dump($bulk);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\BulkWriteCommand)#%d (%d) {
  ["bypassDocumentValidation"]=>
  bool(true)
  ["comment"]=>
  string(3) "foo"
  ["let"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    int(1)
  }
  ["ordered"]=>
  bool(true)
  ["verboseResults"]=>
  bool(true)
  ["session"]=>
  NULL
}
===DONE===
