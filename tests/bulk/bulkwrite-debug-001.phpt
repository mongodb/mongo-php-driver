--TEST--
MongoDB\Driver\BulkWrite debug output before execution
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
    var_dump(new MongoDB\Driver\BulkWrite($options));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\BulkWrite)#%d (%d) {
  ["database"]=>
  NULL
  ["collection"]=>
  NULL
  ["ordered"]=>
  bool(true)
  ["bypassDocumentValidation"]=>
  NULL
  ["executed"]=>
  bool(false)
  ["server_id"]=>
  int(0)
  ["session"]=>
  NULL
  ["write_concern"]=>
  NULL
}
object(MongoDB\Driver\BulkWrite)#%d (%d) {
  ["database"]=>
  NULL
  ["collection"]=>
  NULL
  ["ordered"]=>
  bool(true)
  ["bypassDocumentValidation"]=>
  NULL
  ["executed"]=>
  bool(false)
  ["server_id"]=>
  int(0)
  ["session"]=>
  NULL
  ["write_concern"]=>
  NULL
}
object(MongoDB\Driver\BulkWrite)#%d (%d) {
  ["database"]=>
  NULL
  ["collection"]=>
  NULL
  ["ordered"]=>
  bool(false)
  ["bypassDocumentValidation"]=>
  NULL
  ["executed"]=>
  bool(false)
  ["server_id"]=>
  int(0)
  ["session"]=>
  NULL
  ["write_concern"]=>
  NULL
}
object(MongoDB\Driver\BulkWrite)#%d (%d) {
  ["database"]=>
  NULL
  ["collection"]=>
  NULL
  ["ordered"]=>
  bool(true)
  ["bypassDocumentValidation"]=>
  bool(true)
  ["executed"]=>
  bool(false)
  ["server_id"]=>
  int(0)
  ["session"]=>
  NULL
  ["write_concern"]=>
  NULL
}
object(MongoDB\Driver\BulkWrite)#%d (%d) {
  ["database"]=>
  NULL
  ["collection"]=>
  NULL
  ["ordered"]=>
  bool(true)
  ["bypassDocumentValidation"]=>
  bool(false)
  ["executed"]=>
  bool(false)
  ["server_id"]=>
  int(0)
  ["session"]=>
  NULL
  ["write_concern"]=>
  NULL
}
object(MongoDB\Driver\BulkWrite)#%d (%d) {
  ["database"]=>
  NULL
  ["collection"]=>
  NULL
  ["ordered"]=>
  bool(true)
  ["bypassDocumentValidation"]=>
  NULL
  ["comment"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    int(1)
  }
  ["executed"]=>
  bool(false)
  ["server_id"]=>
  int(0)
  ["session"]=>
  NULL
  ["write_concern"]=>
  NULL
}
object(MongoDB\Driver\BulkWrite)#%d (%d) {
  ["database"]=>
  NULL
  ["collection"]=>
  NULL
  ["ordered"]=>
  bool(true)
  ["bypassDocumentValidation"]=>
  NULL
  ["let"]=>
  object(stdClass)#%d (%d) {
    ["id"]=>
    int(1)
    ["x"]=>
    string(3) "foo"
  }
  ["executed"]=>
  bool(false)
  ["server_id"]=>
  int(0)
  ["session"]=>
  NULL
  ["write_concern"]=>
  NULL
}
===DONE===
