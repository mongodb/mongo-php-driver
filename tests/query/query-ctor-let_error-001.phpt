--TEST--
MongoDB\Driver\Query::__construct(): let option invalid type
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$invalidValues = [true, 1, 'string', null];

foreach ($invalidValues as $invalidValue) {
    echo throws(function() use ($invalidValue) {
        new MongoDB\Driver\Query([], ['let' => $invalidValue]);
    }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "let" option to be array or object, bool given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "let" option to be array or object, int given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "let" option to be array or object, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "let" option to be array or object, null given
===DONE===
