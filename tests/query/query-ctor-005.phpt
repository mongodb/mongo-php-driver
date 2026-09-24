--TEST--
MongoDB\Driver\Query construction with negative limit
--FILE--
<?php

/* The server expects a non-negative limit, but neither PHPC nor libmongoc
 * performs any validation. The option is passed as-is. */
var_dump(new MongoDB\Driver\Query(
    ['x' => 1],
    ['limit' => -5]
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
    int(-5)
  }
  ["readConcern"]=>
  NULL
}
===DONE===
