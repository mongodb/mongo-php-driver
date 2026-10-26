--TEST--
MongoDB\Driver\BulkWriteCommand debug output before execution
--FILE--
<?php

$tests = [
    [],
    ['ordered' => true],
    ['ordered' => false],
    ['bypassDocumentValidation' => true],
    ['bypassDocumentValidation' => false],
    ['comment' => ['foo' => 1]],
    ['let' => ['id' => 1, 'x' => 'foo']],
];

foreach ($tests as $options) {
    var_dump(new MongoDB\Driver\BulkWriteCommand($options));
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
  NULL
}
object(MongoDB\Driver\BulkWriteCommand)#%d (%d) {
  ["bypassDocumentValidation"]=>
  NULL
  ["ordered"]=>
  bool(false)
  ["verboseResults"]=>
  bool(false)
  ["session"]=>
  NULL
}
object(MongoDB\Driver\BulkWriteCommand)#%d (%d) {
  ["bypassDocumentValidation"]=>
  bool(true)
  ["ordered"]=>
  bool(true)
  ["verboseResults"]=>
  bool(false)
  ["session"]=>
  NULL
}
object(MongoDB\Driver\BulkWriteCommand)#%d (%d) {
  ["bypassDocumentValidation"]=>
  bool(false)
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
  ["comment"]=>
  object(stdClass)#2 (1) {
    ["foo"]=>
    int(1)
  }
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
  ["let"]=>
  object(stdClass)#2 (2) {
    ["id"]=>
    int(1)
    ["x"]=>
    string(3) "foo"
  }
  ["ordered"]=>
  bool(true)
  ["verboseResults"]=>
  bool(false)
  ["session"]=>
  NULL
}
===DONE===
