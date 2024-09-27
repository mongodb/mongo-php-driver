--TEST--
PHPC-2457: Query options can be passed reference
--FILE--
<?php

$collation = ['locale' => 'fr_FR', 'strength' => 2];
$let = ['x' => 1];
$sort = ['_id' => 1];

$query = new MongoDB\Driver\Query([], [
    'collation' => &$collation,
    'let' => &$let,
    'sort' => &$sort,
]);

var_dump($query);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
object(MongoDB\Driver\Query)#1 (3) {
  ["filter"]=>
  object(stdClass)#2 (0) {
  }
  ["options"]=>
  object(stdClass)#6 (3) {
    ["collation"]=>
    object(stdClass)#3 (2) {
      ["locale"]=>
      string(5) "fr_FR"
      ["strength"]=>
      int(2)
    }
    ["let"]=>
    object(stdClass)#4 (1) {
      ["x"]=>
      int(1)
    }
    ["sort"]=>
    object(stdClass)#5 (1) {
      ["_id"]=>
      int(1)
    }
  }
  ["readConcern"]=>
  NULL
}
===DONE===
