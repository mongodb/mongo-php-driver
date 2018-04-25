--TEST--
Cursor::setTypeMap(): Type classes must be instantiatable and implement Unserializable
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

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

$manager = new MongoDB\Driver\Manager(URI);
$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));

foreach ($types as $type) {
    foreach ($classes as $class) {
        $typeMap = [$type => $class];

        printf("Test typeMap: %s\n", json_encode($typeMap));

        echo throws(function() use ($cursor, $typeMap) {
            $cursor->setTypeMap($typeMap);
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
