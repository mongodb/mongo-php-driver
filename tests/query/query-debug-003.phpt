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

Deprecated: MongoDB\Driver\Query::__construct(): The "modifiers" option is deprecated and will be removed in a future release in %s on line %d
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
