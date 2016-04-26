--TEST--
MongoDB\Driver\BulkWrite debug output before execution
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$tests = [
    [],
    ['ordered' => true],
    ['ordered' => false],
    ['bypassDocumentValidation' => true],
    ['bypassDocumentValidation' => false],
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
  ["write_concern"]=>
  NULL
}
===DONE===
