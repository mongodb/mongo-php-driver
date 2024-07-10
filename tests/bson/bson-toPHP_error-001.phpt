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
            MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{}'), $typeMap);
        }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n\n";
    }
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Test typeMap: {"array":"MissingClass"}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist

Test typeMap: {"document":"MissingClass"}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist

Test typeMap: {"root":"MissingClass"}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist

Test typeMap: {"fieldPaths":{"x":"MissingClass"}}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist

Test typeMap: {"array":"MyAbstractDocument"}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyAbstractDocument is not instantiatable

Test typeMap: {"document":"MyAbstractDocument"}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyAbstractDocument is not instantiatable

Test typeMap: {"root":"MyAbstractDocument"}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyAbstractDocument is not instantiatable

Test typeMap: {"fieldPaths":{"x":"MyAbstractDocument"}}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyAbstractDocument is not instantiatable

Test typeMap: {"array":"MyDocument"}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyDocument does not implement MongoDB\BSON\Unserializable

Test typeMap: {"document":"MyDocument"}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyDocument does not implement MongoDB\BSON\Unserializable

Test typeMap: {"root":"MyDocument"}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyDocument does not implement MongoDB\BSON\Unserializable

Test typeMap: {"fieldPaths":{"x":"MyDocument"}}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyDocument does not implement MongoDB\BSON\Unserializable

Test typeMap: {"array":"MyTrait"}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Trait MyTrait is not instantiatable

Test typeMap: {"document":"MyTrait"}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Trait MyTrait is not instantiatable

Test typeMap: {"root":"MyTrait"}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Trait MyTrait is not instantiatable

Test typeMap: {"fieldPaths":{"x":"MyTrait"}}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Trait MyTrait is not instantiatable

Test typeMap: {"array":"MongoDB\\BSON\\Unserializable"}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Interface MongoDB\BSON\Unserializable is not instantiatable

Test typeMap: {"document":"MongoDB\\BSON\\Unserializable"}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Interface MongoDB\BSON\Unserializable is not instantiatable

Test typeMap: {"root":"MongoDB\\BSON\\Unserializable"}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Interface MongoDB\BSON\Unserializable is not instantiatable

Test typeMap: {"fieldPaths":{"x":"MongoDB\\BSON\\Unserializable"}}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Interface MongoDB\BSON\Unserializable is not instantiatable

===DONE===
