--TEST--
MongoDB\Driver\Query debug output with let option
--FILE--
<?php

var_dump(new MongoDB\Driver\Query(
    ['$expr' => ['$eq' => ['$_id', '$$id']]],
    ['let' => ['id' => 1]]
));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\Query)#%d (%d) {
  ["filter"]=>
  object(stdClass)#%d (%d) {
    ["$expr"]=>
    object(stdClass)#%d (%d) {
      ["$eq"]=>
      array(2) {
        [0]=>
        string(4) "$_id"
        [1]=>
        string(4) "$$id"
      }
    }
  }
  ["options"]=>
  object(stdClass)#%d (%d) {
    ["let"]=>
    object(stdClass)#%d (%d) {
      ["id"]=>
      int(1)
    }
  }
  ["readConcern"]=>
  NULL
}
===DONE===
