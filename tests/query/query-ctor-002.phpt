--TEST--
MongoDB\Driver\Query construction with options
--FILE--
<?php

var_dump(new MongoDB\Driver\Query(
    ['x' => 1],
    [
        'allowPartialResults' => false,
        'awaitData' => false,
        'batchSize' => 10,
        'collation' => ['locale' => 'en_US'],
        'comment' => 'foo',
        'exhaust' => false,
        'limit' => 20,
        'max' => ['y' => 100],
        'maxScan' => 50,
        'maxTimeMS' => 1000,
        'min' => ['y' => 1],
        'noCursorTimeout' => false,
        'oplogReplay' => false,
        'projection' => ['x' => 1, 'y' => 1],
        'returnKey' => false,
        'showRecordId' => false,
        'singleBatch' => false,
        'skip' => 5,
        'sort' => ['y' => -1],
        'snapshot' => false,
        'tailable' => false,
    ]
));

var_dump(new MongoDB\Driver\Query(
    ['x' => 1],
    ['hint' => 'y_1']
));

var_dump(new MongoDB\Driver\Query(
    ['x' => 1],
    ['hint' => ['y' => 1]]
));

var_dump(new MongoDB\Driver\Query(
    ['x' => 1],
    ['readConcern' => new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LOCAL)]
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
    ["allowPartialResults"]=>
    bool(false)
    ["awaitData"]=>
    bool(false)
    ["batchSize"]=>
    int(10)
    ["collation"]=>
    object(stdClass)#%d (%d) {
      ["locale"]=>
      string(5) "en_US"
    }
    ["comment"]=>
    string(3) "foo"
    ["exhaust"]=>
    bool(false)
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
    ["noCursorTimeout"]=>
    bool(false)
    ["oplogReplay"]=>
    bool(false)
    ["projection"]=>
    object(stdClass)#%d (%d) {
      ["x"]=>
      int(1)
      ["y"]=>
      int(1)
    }
    ["returnKey"]=>
    bool(false)
    ["showRecordId"]=>
    bool(false)
    ["skip"]=>
    int(5)
    ["sort"]=>
    object(stdClass)#%d (%d) {
      ["y"]=>
      int(-1)
    }
    ["snapshot"]=>
    bool(false)
    ["tailable"]=>
    bool(false)
    ["limit"]=>
    int(20)
    ["singleBatch"]=>
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
object(MongoDB\Driver\Query)#%d (%d) {
  ["filter"]=>
  object(stdClass)#%d (%d) {
    ["x"]=>
    int(1)
  }
  ["options"]=>
  object(stdClass)#%d (%d) {
  }
  ["readConcern"]=>
  array(1) {
    ["level"]=>
    string(5) "local"
  }
}
===DONE===
