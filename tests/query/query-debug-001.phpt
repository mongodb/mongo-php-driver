--TEST--
MongoDB\Driver\Query debug output
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

var_dump(new MongoDB\Driver\Query(
    ['a' => 123],
    [
        'limit' => 5,
        'modifiers' => [
            '$comment' => 'foo',
            '$maxTimeMS' => 500,
        ],
        'projection' => ['c' => 1],
        'readConcern' => new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LOCAL),
        'skip' => 10,
        'sort' => ['b' => -1],
    ]
));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\Query)#%d (%d) {
  ["query"]=>
  object(stdClass)#%d (%d) {
    ["$comment"]=>
    string(3) "foo"
    ["$maxTimeMS"]=>
    int(500)
    ["$orderby"]=>
    object(stdClass)#%d (%d) {
      ["b"]=>
      int(-1)
    }
    ["$query"]=>
    object(stdClass)#%d (%d) {
      ["a"]=>
      int(123)
    }
  }
  ["selector"]=>
  object(stdClass)#%d (%d) {
    ["c"]=>
    int(1)
  }
  ["flags"]=>
  int(0)
  ["skip"]=>
  int(10)
  ["limit"]=>
  int(5)
  ["batch_size"]=>
  int(0)
  ["readConcern"]=>
  array(1) {
    ["level"]=>
    string(5) "local"
  }
}
===DONE===
