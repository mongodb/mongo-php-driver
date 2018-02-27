--TEST--
PHPC-430: Query constructor arguments are modified
--FILE--
<?php

$filter = [];
$options = ['sort' => ['x' => 1]];
$query = new MongoDB\Driver\Query($filter, $options);

var_dump($filter);
var_dump($options);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(0) {
}
array(1) {
  ["sort"]=>
  array(1) {
    ["x"]=>
    int(1)
  }
}
===DONE===
