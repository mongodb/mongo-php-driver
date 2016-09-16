--TEST--
MongoDB\Driver\Query construction with negative limit
--FILE--
<?php

var_dump(new MongoDB\Driver\Query(
    ['x' => 1],
    ['limit' => -5]
));

var_dump(new MongoDB\Driver\Query(
    ['x' => 1],
    [
        'limit' => -5,
        'singleBatch' => true
    ]
));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\Query)#%d (%d) {
  ["filter"]=>
  object(stdClass)#%d (%d) {
    ["x"]=>
    int(1)
  }
  ["options"]=>
  object(stdClass)#%d (%d) {
    ["limit"]=>
    int(5)
    ["singleBatch"]=>
    bool(true)
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
    ["limit"]=>
    int(5)
    ["singleBatch"]=>
    bool(true)
  }
  ["readConcern"]=>
  NULL
}
===DONE===
