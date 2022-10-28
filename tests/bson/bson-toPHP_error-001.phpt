--TEST--
MongoDB\BSON\toPHP(): Type map classes must be instantiatable and implement Unserializable
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

abstract class MyAbstractDocument implements MongoDB\BSON\Unserializable {}

class MyDocument {}

trait MyTrait {}

$classes = [
    'MissingClass',
    MyAbstractDocument::class,
    MyDocument::class,
    MyTrait::class,
    MongoDB\BSON\Unserializable::class,
];

foreach ($classes as $class) {
    $typeMaps = [
        ['array' => $class],
        ['document' => $class],
        ['root' => $class],
        ['fieldPaths' => ['x' => $class]],
    ];

    foreach ($typeMaps as $typeMap) {
        printf("Test typeMap: %s\n", json_encode($typeMap));

        echo throws(function() use ($typeMap) {
            toPHP(fromJSON('{}'), $typeMap);
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

Test typeMap: {"array":"MyTrait"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Trait MyTrait is not instantiatable

Test typeMap: {"document":"MyTrait"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Trait MyTrait is not instantiatable

Test typeMap: {"root":"MyTrait"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Trait MyTrait is not instantiatable

Test typeMap: {"fieldPaths":{"x":"MyTrait"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Trait MyTrait is not instantiatable

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
