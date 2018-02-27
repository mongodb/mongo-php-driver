--TEST--
PHPC-430: Query constructor arguments are modified
--FILE--
<?php

function buildQuery($filter, $options)
{
    $options["cursorFlags"] = 0;
    return new MongoDB\Driver\Query($filter, $options);
}

$filter = [];
$options = ['sort' => []];
$query = buildQuery($filter, $options);

var_dump($options);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(1) {
  ["sort"]=>
  array(0) {
  }
}
===DONE===
