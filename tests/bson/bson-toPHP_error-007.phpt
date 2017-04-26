--TEST--
MongoDB\BSON\toPHP(): Type wrappers must be instantiatable and implement TypeWrapper
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

abstract class MyAbstractDocument implements MongoDB\BSON\TypeWrapper {}

class MyDocument {}

$types = [
    'Binary',
    'Decimal128',
    'Javascript',
    'MaxKey',
    'MinKey',
    'ObjectID',
    'Regex',
    'Timestamp',
    'UTCDateTime',
];

$classes = [
    'MissingClass',
    'MyAbstractDocument',
    'MyDocument',
    'MongoDB\BSON\TypeWrapper',
];

$bson = pack('Vx', 5); // Empty document

foreach ($types as $type) {
    foreach ($classes as $class) {
        $typeMap = ['types' => [$type => $class]];

        printf("Test typeMap: %s\n", json_encode($typeMap));

        echo throws(function() use ($bson, $typeMap) {
            toPHP($bson, $typeMap);
        }, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

        echo "\n";
    }
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Test typeMap: {"types":{"Binary":"MissingClass"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist

Test typeMap: {"types":{"Binary":"MyAbstractDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyAbstractDocument is not instantiatable

Test typeMap: {"types":{"Binary":"MyDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyDocument does not implement MongoDB\BSON\TypeWrapper

Test typeMap: {"types":{"Binary":"MongoDB\\BSON\\TypeWrapper"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MongoDB\BSON\TypeWrapper is not instantiatable

Test typeMap: {"types":{"Decimal128":"MissingClass"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist

Test typeMap: {"types":{"Decimal128":"MyAbstractDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyAbstractDocument is not instantiatable

Test typeMap: {"types":{"Decimal128":"MyDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyDocument does not implement MongoDB\BSON\TypeWrapper

Test typeMap: {"types":{"Decimal128":"MongoDB\\BSON\\TypeWrapper"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MongoDB\BSON\TypeWrapper is not instantiatable

Test typeMap: {"types":{"Javascript":"MissingClass"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist

Test typeMap: {"types":{"Javascript":"MyAbstractDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyAbstractDocument is not instantiatable

Test typeMap: {"types":{"Javascript":"MyDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyDocument does not implement MongoDB\BSON\TypeWrapper

Test typeMap: {"types":{"Javascript":"MongoDB\\BSON\\TypeWrapper"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MongoDB\BSON\TypeWrapper is not instantiatable

Test typeMap: {"types":{"MaxKey":"MissingClass"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist

Test typeMap: {"types":{"MaxKey":"MyAbstractDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyAbstractDocument is not instantiatable

Test typeMap: {"types":{"MaxKey":"MyDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyDocument does not implement MongoDB\BSON\TypeWrapper

Test typeMap: {"types":{"MaxKey":"MongoDB\\BSON\\TypeWrapper"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MongoDB\BSON\TypeWrapper is not instantiatable

Test typeMap: {"types":{"MinKey":"MissingClass"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist

Test typeMap: {"types":{"MinKey":"MyAbstractDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyAbstractDocument is not instantiatable

Test typeMap: {"types":{"MinKey":"MyDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyDocument does not implement MongoDB\BSON\TypeWrapper

Test typeMap: {"types":{"MinKey":"MongoDB\\BSON\\TypeWrapper"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MongoDB\BSON\TypeWrapper is not instantiatable

Test typeMap: {"types":{"ObjectID":"MissingClass"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist

Test typeMap: {"types":{"ObjectID":"MyAbstractDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyAbstractDocument is not instantiatable

Test typeMap: {"types":{"ObjectID":"MyDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyDocument does not implement MongoDB\BSON\TypeWrapper

Test typeMap: {"types":{"ObjectID":"MongoDB\\BSON\\TypeWrapper"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MongoDB\BSON\TypeWrapper is not instantiatable

Test typeMap: {"types":{"Regex":"MissingClass"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist

Test typeMap: {"types":{"Regex":"MyAbstractDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyAbstractDocument is not instantiatable

Test typeMap: {"types":{"Regex":"MyDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyDocument does not implement MongoDB\BSON\TypeWrapper

Test typeMap: {"types":{"Regex":"MongoDB\\BSON\\TypeWrapper"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MongoDB\BSON\TypeWrapper is not instantiatable

Test typeMap: {"types":{"Timestamp":"MissingClass"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist

Test typeMap: {"types":{"Timestamp":"MyAbstractDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyAbstractDocument is not instantiatable

Test typeMap: {"types":{"Timestamp":"MyDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyDocument does not implement MongoDB\BSON\TypeWrapper

Test typeMap: {"types":{"Timestamp":"MongoDB\\BSON\\TypeWrapper"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MongoDB\BSON\TypeWrapper is not instantiatable

Test typeMap: {"types":{"UTCDateTime":"MissingClass"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist

Test typeMap: {"types":{"UTCDateTime":"MyAbstractDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyAbstractDocument is not instantiatable

Test typeMap: {"types":{"UTCDateTime":"MyDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyDocument does not implement MongoDB\BSON\TypeWrapper

Test typeMap: {"types":{"UTCDateTime":"MongoDB\\BSON\\TypeWrapper"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MongoDB\BSON\TypeWrapper is not instantiatable

===DONE===
