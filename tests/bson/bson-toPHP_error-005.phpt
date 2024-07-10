--TEST--
MongoDB\BSON\toPHP(): Field path values with bson_iter_visit_all() failures
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = array(
    str_replace('INVALID!', "INVALID\xFE", MongoDB\BSON\fromPHP(['foo' => ['INVALID!' => 'bar'] ])),
    str_replace('INVALID!', "INVALID\xFE", MongoDB\BSON\fromPHP(['foo' => ['bar' => ['INVALID!' => 'bar']]])),
    str_replace('INVALID!', "INVALID\xFE", MongoDB\BSON\fromPHP(['foo' => ['bar' => ['INVALID!']]])),
    str_replace('INVALID!', "INVALID\xFE", MongoDB\BSON\fromPHP(['foo' => [['INVALID!']]])),
    str_replace('INVALID!', "INVALID\xFE", MongoDB\BSON\fromPHP(['foo' => [ ['bar' => ['INVALID!' => 'bar']], 6 ]])),
);

foreach ($tests as $bson) {
    echo throws(function() use ($bson) {
        MongoDB\BSON\toPHP($bson);
    }, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected corrupt BSON data for field path 'foo' at offset 0

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected corrupt BSON data for field path 'foo.bar' at offset 0

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected corrupt BSON data for field path 'foo.bar' at offset 0

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected corrupt BSON data for field path 'foo.0' at offset 0

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected corrupt BSON data for field path 'foo.0.bar' at offset 0
===DONE===
