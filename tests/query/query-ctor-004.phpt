--TEST--
MongoDB\Driver\Query construction with options overriding modifiers
--FILE--
<?php

var_dump(new MongoDB\Driver\Query(
    ['x' => 1],
    [
        'comment' => 'foo',
        'max' => ['y' => 100],
        'maxScan' => 50,
        'maxTimeMS' => 1000,
        'min' => ['y' => 1],
        'returnKey' => false,
        'showRecordId' => false,
        'sort' => ['y' => -1],
        'snapshot' => false,
        'modifiers' => [
            '$comment' => 'bar',
            '$max' => ['y' => 200],
            '$maxScan' => 60,
            '$maxTimeMS' => 2000,
            '$min' => ['y' => 101],
            '$orderby' => ['y' => 1],
            '$returnKey' => true,
            '$showDiskLoc' => true,
            '$snapshot' => true,
        ],
    ]
));

var_dump(new MongoDB\Driver\Query(
    ['x' => 1],
    [
        'hint' => 'y_1',
        'modifiers' => ['$hint' => 'x_1'],
    ]
));

var_dump(new MongoDB\Driver\Query(
    ['x' => 1],
    [
        'hint' => ['y' => 1],
        'modifiers' => ['$hint' => ['x' => 1]],
    ]
));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: MongoDB\Driver\Query::__construct(): The "maxScan" option is deprecated and will be removed in a future release in %s on line %d

Deprecated: MongoDB\Driver\Query::__construct(): The "snapshot" option is deprecated and will be removed in a future release in %s on line %d
object(MongoDB\Driver\Query)#%d (%d) {
  ["filter"]=>
  object(stdClass)#%d (%d) {
    ["x"]=>
    int(1)
  }
  ["options"]=>
  object(stdClass)#%d (%d) {
    ["comment"]=>
    string(3) "foo"
    ["max"]=>
    object(stdClass)#%d (%d) {
      ["y"]=>
      int(100)
    }
    ["maxScan"]=>
    int(50)
    ["maxTimeMS"]=>
    int(1000)
    ["min"]=>
    object(stdClass)#%d (%d) {
      ["y"]=>
      int(1)
    }
    ["returnKey"]=>
    bool(false)
    ["showRecordId"]=>
    bool(false)
    ["sort"]=>
    object(stdClass)#%d (%d) {
      ["y"]=>
      int(-1)
    }
    ["snapshot"]=>
    bool(false)
  }
  ["readConcern"]=>
  NULL
}
object(MongoDB\Driver\Query)#%d (%d) {
  ["filter"]=>
  object(stdClass)#%d (%d) {
    ["x"]=>
    int(1)
  }
  ["options"]=>
  object(stdClass)#%d (%d) {
    ["hint"]=>
    string(3) "y_1"
  }
  ["readConcern"]=>
  NULL
}
object(MongoDB\Driver\Query)#%d (%d) {
  ["filter"]=>
  object(stdClass)#%d (%d) {
    ["x"]=>
    int(1)
  }
  ["options"]=>
  object(stdClass)#%d (%d) {
    ["hint"]=>
    object(stdClass)#%d (%d) {
      ["y"]=>
      int(1)
    }
  }
  ["readConcern"]=>
  NULL
}
===DONE===
