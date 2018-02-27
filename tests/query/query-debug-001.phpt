--TEST--
MongoDB\Driver\Query debug output
--FILE--
<?php

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
  ["filter"]=>
  object(stdClass)#%d (%d) {
    ["a"]=>
    int(123)
  }
  ["options"]=>
  object(stdClass)#%d (%d) {
    ["comment"]=>
    string(3) "foo"
    ["maxTimeMS"]=>
    int(500)
    ["projection"]=>
    object(stdClass)#%d (%d) {
      ["c"]=>
      int(1)
    }
    ["skip"]=>
    int(10)
    ["sort"]=>
    object(stdClass)#%d (%d) {
      ["b"]=>
      int(-1)
    }
    ["limit"]=>
    int(5)
  }
  ["readConcern"]=>
  array(1) {
    ["level"]=>
    string(5) "local"
  }
}
===DONE===
