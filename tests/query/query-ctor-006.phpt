--TEST--
MongoDB\Driver\Query construction "allowPartialResults" overrides "partial" option
--FILE--
<?php

var_dump(new MongoDB\Driver\Query(
    ['x' => 1],
    ['partial' => true]
));

var_dump(new MongoDB\Driver\Query(
    ['x' => 1],
    [
        'allowPartialResults' => false,
        'partial' => true,
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
    ["allowPartialResults"]=>
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
    ["allowPartialResults"]=>
    bool(false)
  }
  ["readConcern"]=>
  NULL
}
===DONE===
