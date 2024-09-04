--TEST--
MongoDB\BSON\toPHP(): Type map classes must be instantiatable and implement Unserializable (enums)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.1.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

enum MyEnum
{
    case A;
}

enum MyBackedEnum: int
{
    case A = 1;
}

$classes = [
    MyEnum::class,
    MyBackedEnum::class,
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
Test typeMap: {"array":"MyEnum"}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Enum MyEnum is not instantiatable

Test typeMap: {"document":"MyEnum"}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Enum MyEnum is not instantiatable

Test typeMap: {"root":"MyEnum"}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Enum MyEnum is not instantiatable

Test typeMap: {"fieldPaths":{"x":"MyEnum"}}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Enum MyEnum is not instantiatable

Test typeMap: {"array":"MyBackedEnum"}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Enum MyBackedEnum is not instantiatable

Test typeMap: {"document":"MyBackedEnum"}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Enum MyBackedEnum is not instantiatable

Test typeMap: {"root":"MyBackedEnum"}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Enum MyBackedEnum is not instantiatable

Test typeMap: {"fieldPaths":{"x":"MyBackedEnum"}}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Enum MyBackedEnum is not instantiatable

===DONE===
