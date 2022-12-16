--TEST--
MongoDB\BSON\Document::fromBSON(): Field path values with bson_iter_visit_all() failures
--DESCRIPTION--
This test was adapted from bson-toPHP_error-005.phpt
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    str_replace('INVALID!', "INVALID\xFE", fromPHP(['foo' => ['INVALID!' => 'bar'] ])),
    str_replace('INVALID!', "INVALID\xFE", fromPHP(['foo' => ['bar' => ['INVALID!' => 'bar']]])),
    str_replace('INVALID!', "INVALID\xFE", fromPHP(['foo' => ['bar' => ['INVALID!']]])),
    str_replace('INVALID!', "INVALID\xFE", fromPHP(['foo' => [['INVALID!']]])),
    str_replace('INVALID!', "INVALID\xFE", fromPHP(['foo' => [ ['bar' => ['INVALID!' => 'bar']], 6 ]])),
];

foreach ($tests as $bson) {
    echo throws(function() use ($bson) {
        MongoDB\BSON\Document::fromBSON($bson)->toPHP();
    }, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected corrupt BSON data for field path 'foo' at offset 0
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected corrupt BSON data for field path 'foo.bar' at offset 0
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected corrupt BSON data for field path 'foo.bar' at offset 0
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected corrupt BSON data for field path 'foo.0' at offset 0
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected corrupt BSON data for field path 'foo.0.bar' at offset 0
===DONE===
