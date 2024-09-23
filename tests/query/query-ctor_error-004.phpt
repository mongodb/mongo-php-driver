--TEST--
MongoDB\Driver\Query construction (cannot use empty keys in documents)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    [['' => '1'], []],
    [['x' => ['' => '1']], []],
    [[], ['collation' => ['' => 1]]],
    [[], ['hint' => ['' => 1]]],
    [[], ['max' => ['' => 1]]],
    [[], ['min' => ['' => 1]]],
    [[], ['projection' => ['' => 1]]],
    [[], ['sort' => ['' => 1]]],
    [[], ['modifiers' => ['$hint' => ['' => 1]]]],
    [[], ['modifiers' => ['$max' => ['' => 1]]]],
    [[], ['modifiers' => ['$min' => ['' => 1]]]],
    [[], ['modifiers' => ['$orderby' => ['' => 1]]]],
];

foreach ($tests as $test) {
    list($filter, $options) = $test;

    echo throws(function() use ($filter, $options) {
        new MongoDB\Driver\Query($filter, $options);
    }, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot use empty keys in filter document
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot use empty keys in filter document
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot use empty keys in "collation" option
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot use empty keys in "hint" option
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot use empty keys in "max" option
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot use empty keys in "min" option
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot use empty keys in "projection" option
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot use empty keys in "sort" option

Deprecated: MongoDB\Driver\Query::__construct(): The "modifiers" option is deprecated and will be removed in a future release in %s on line %d
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot use empty keys in "$hint" modifier

Deprecated: MongoDB\Driver\Query::__construct(): The "modifiers" option is deprecated and will be removed in a future release in %s on line %d
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot use empty keys in "$max" modifier

Deprecated: MongoDB\Driver\Query::__construct(): The "modifiers" option is deprecated and will be removed in a future release in %s on line %d
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot use empty keys in "$min" modifier

Deprecated: MongoDB\Driver\Query::__construct(): The "modifiers" option is deprecated and will be removed in a future release in %s on line %d
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot use empty keys in "$orderby" modifier
===DONE===
