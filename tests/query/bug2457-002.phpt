--TEST--
PHPC-2457: Query modifiers can be passed reference
--FILE--
<?php

$modifiers = ['$orderby' => ['x' => 1]];

$query = new MongoDB\Driver\Query([], [
    'modifiers' => &$modifiers,
]);

var_dump($query);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: MongoDB\Driver\Query::__construct(): The "modifiers" option is deprecated and will be removed in a future release in %s
object(MongoDB\Driver\Query)#1 (3) {
  ["filter"]=>
  object(stdClass)#2 (0) {
  }
  ["options"]=>
  object(stdClass)#4 (1) {
    ["sort"]=>
    object(stdClass)#3 (1) {
      ["x"]=>
      int(1)
    }
  }
  ["readConcern"]=>
  NULL
}
===DONE===
