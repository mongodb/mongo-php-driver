--TEST--
MongoDB\Driver\Query construction (invalid option types)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    ['collation' => 0],
    ['hint' => 0],
    ['max' => 0],
    ['min' => 0],
    ['projection' => 0],
    ['sort' => 0],
];

foreach ($tests as $options) {
    echo throws(function() use ($options) {
        new MongoDB\Driver\Query([], $options);
    }, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "collation" option to be array or object, int given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "hint" option to be string, array, or object, int given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "max" option to be array or object, int given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "min" option to be array or object, int given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "projection" option to be array or object, int given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "sort" option to be array or object, int given
===DONE===
