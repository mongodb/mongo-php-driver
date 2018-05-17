--TEST--
MongoDB\Driver\Query construction (invalid option types)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    ['modifiers' => 0],
    ['collation' => 0],
    ['comment' => 0],
    ['hint' => 0],
    ['max' => 0],
    ['min' => 0],
    ['projection' => 0],
    ['sort' => 0],
    ['modifiers' => ['$comment' => 0]],
    ['modifiers' => ['$hint' => 0]],
    ['modifiers' => ['$max' => 0]],
    ['modifiers' => ['$min' => 0]],
    ['modifiers' => ['$orderby' => 0]],
];

foreach ($tests as $options) {
    echo throws(function() use ($options) {
        new MongoDB\Driver\Query([], $options);
    }, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "modifiers" option to be array, int%S given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "collation" option to be array or object, int%S given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "comment" option to be string, int%S given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "hint" option to be string, array, or object, int%S given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "max" option to be array or object, int%S given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "min" option to be array or object, int%S given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "projection" option to be array or object, int%S given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "sort" option to be array or object, int%S given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "$comment" modifier to be string, int%S given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "$hint" modifier to be string, array, or object, int%S given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "$max" modifier to be array or object, int%S given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "$min" modifier to be array or object, int%S given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "$orderby" modifier to be array or object, int%S given

===DONE===
