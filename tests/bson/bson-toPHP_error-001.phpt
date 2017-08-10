--TEST--
MongoDB\BSON\toPHP(): Type classes must be instantiatable and implement Unserializable
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

abstract class MyAbstractDocument implements MongoDB\BSON\Unserializable {}

class MyDocument {}

$types = [
    'array',
    'document',
    'root',
];

$classes = [
    'MissingClass',
    'MyAbstractDocument',
    'MyDocument',
    'MongoDB\BSON\Unserializable',
];

$bson = pack('Vx', 5); // Empty document

foreach ($types as $type) {
    foreach ($classes as $class) {
        $typeMap = [$type => $class];

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
Test typeMap: {"array":"MissingClass"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist

Test typeMap: {"array":"MyAbstractDocument"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyAbstractDocument is not instantiatable

Test typeMap: {"array":"MyDocument"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyDocument does not implement MongoDB\BSON\Unserializable

Test typeMap: {"array":"MongoDB\\BSON\\Unserializable"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MongoDB\BSON\Unserializable is not instantiatable

Test typeMap: {"document":"MissingClass"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist

Test typeMap: {"document":"MyAbstractDocument"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyAbstractDocument is not instantiatable

Test typeMap: {"document":"MyDocument"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyDocument does not implement MongoDB\BSON\Unserializable

Test typeMap: {"document":"MongoDB\\BSON\\Unserializable"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MongoDB\BSON\Unserializable is not instantiatable

Test typeMap: {"root":"MissingClass"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist

Test typeMap: {"root":"MyAbstractDocument"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyAbstractDocument is not instantiatable

Test typeMap: {"root":"MyDocument"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyDocument does not implement MongoDB\BSON\Unserializable

Test typeMap: {"root":"MongoDB\\BSON\\Unserializable"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MongoDB\BSON\Unserializable is not instantiatable

===DONE===
