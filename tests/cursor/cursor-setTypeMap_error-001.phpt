--TEST--
Cursor::setTypeMap(): Type map classes must be instantiatable and implement Unserializable
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

abstract class MyAbstractDocument implements MongoDB\BSON\Unserializable {}

class MyDocument {}

/* Note: this test omits traits and enums, although those are tested with
 * MongoDB\BSON\toPHP(), which uses the same type map validation. */
$classes = [
    'MissingClass',
    MyAbstractDocument::class,
    MyDocument::class,
    MongoDB\BSON\Unserializable::class,
];

$manager = create_test_manager();
$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));

foreach ($classes as $class) {
    $typeMaps = [
        ['array' => $class],
        ['document' => $class],
        ['root' => $class],
        ['fieldPaths' => ['x' => $class]],
    ];

     foreach ($typeMaps as $typeMap) {
        printf("Test typeMap: %s\n", json_encode($typeMap));

        echo throws(function() use ($cursor, $typeMap) {
            $cursor->setTypeMap($typeMap);
        }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n\n";
    }
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Test typeMap: {"array":"MissingClass"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist

Test typeMap: {"document":"MissingClass"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist

Test typeMap: {"root":"MissingClass"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist

Test typeMap: {"fieldPaths":{"x":"MissingClass"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist

Test typeMap: {"array":"MyAbstractDocument"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyAbstractDocument is not instantiatable

Test typeMap: {"document":"MyAbstractDocument"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyAbstractDocument is not instantiatable

Test typeMap: {"root":"MyAbstractDocument"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyAbstractDocument is not instantiatable

Test typeMap: {"fieldPaths":{"x":"MyAbstractDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyAbstractDocument is not instantiatable

Test typeMap: {"array":"MyDocument"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyDocument does not implement MongoDB\BSON\Unserializable

Test typeMap: {"document":"MyDocument"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyDocument does not implement MongoDB\BSON\Unserializable

Test typeMap: {"root":"MyDocument"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyDocument does not implement MongoDB\BSON\Unserializable

Test typeMap: {"fieldPaths":{"x":"MyDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyDocument does not implement MongoDB\BSON\Unserializable

Test typeMap: {"array":"MongoDB\\BSON\\Unserializable"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Interface MongoDB\BSON\Unserializable is not instantiatable

Test typeMap: {"document":"MongoDB\\BSON\\Unserializable"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Interface MongoDB\BSON\Unserializable is not instantiatable

Test typeMap: {"root":"MongoDB\\BSON\\Unserializable"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Interface MongoDB\BSON\Unserializable is not instantiatable

Test typeMap: {"fieldPaths":{"x":"MongoDB\\BSON\\Unserializable"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Interface MongoDB\BSON\Unserializable is not instantiatable

===DONE===
