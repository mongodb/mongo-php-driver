--TEST--
MongoDB\Driver\Query debug output with comment option
--FILE--
<?php

var_dump(new MongoDB\Driver\Query(
    [],
    ['comment' => ['foo' => 1]]
));

var_dump(new MongoDB\Driver\Query(
    [],
    ['modifiers' => ['$comment' => ['foo' => 1]]]
));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\Query)#%d (%d) {
  ["filter"]=>
  object(stdClass)#%d (0) {
  }
  ["options"]=>
  object(stdClass)#%d (%d) {
    ["comment"]=>
    object(stdClass)#%d (%d) {
      ["foo"]=>
      int(1)
    }
  }
  ["readConcern"]=>
  NULL
}
object(MongoDB\Driver\Query)#%d (%d) {
  ["filter"]=>
  object(stdClass)#%d (0) {
  }
  ["options"]=>
  object(stdClass)#%d (%d) {
    ["comment"]=>
    object(stdClass)#%d (%d) {
      ["foo"]=>
      int(1)
    }
  }
  ["readConcern"]=>
  NULL
}
===DONE===
