--TEST--
PHPC-2744: Field path for corrupt BSON at a nesting depth that is a multiple of 8
--DESCRIPTION--
The field path element for the current level is stored at index "size", which may
be past the end of the allocation when the depth is a multiple of the allocation
step. Reading it leaked heap memory into the exception message.
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

function nest(int $depth): array
{
    $value = ['INVALID!' => 'bar'];

    for ($i = 0; $i < $depth; $i++) {
        $value = ['a' => $value];
    }

    return $value;
}

foreach ([7, 8, 9, 15, 16, 17] as $depth) {
    $bson = str_replace('INVALID!', "INVALID\xFE", fromPHP(nest($depth)));

    echo throws(function() use ($bson) {
        MongoDB\BSON\Document::fromBSON($bson)->toPHP();
    }, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected corrupt BSON data for field path 'a.a.a.a.a.a.a' at offset 0
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected corrupt BSON data for field path 'a.a.a.a.a.a.a.a' at offset 0
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected corrupt BSON data for field path 'a.a.a.a.a.a.a.a.a' at offset 0
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected corrupt BSON data for field path 'a.a.a.a.a.a.a.a.a.a.a.a.a.a.a' at offset 0
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected corrupt BSON data for field path 'a.a.a.a.a.a.a.a.a.a.a.a.a.a.a.a' at offset 0
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected corrupt BSON data for field path 'a.a.a.a.a.a.a.a.a.a.a.a.a.a.a.a.a' at offset 0
===DONE===
