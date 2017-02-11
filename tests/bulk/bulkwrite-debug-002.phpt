--TEST--
MongoDB\Driver\BulkWrite debug output before execution (__debugInfo)
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
	$bw = new MongoDB\Driver\BulkWrite($options);
    var_dump($bw->__debugInfo());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(7) {
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
array(7) {
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
array(7) {
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
array(7) {
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
array(7) {
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
