--TEST--
Cursor::setTypeMap(): fieldPaths must be an array, with single key/string elements
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

abstract class MyAbstractDocument implements MongoDB\BSON\Unserializable {}

class MyDocument {}

$fieldPaths = [
    'notAnArray',
    ['notAssociative'],
    ['missing' => 'MissingClass'],
    ['abstract' => 'MyAbstractDocument'],
    ['my' => 'MyDocument'],
    ['unserialize' => 'MongoDB\BSON\Unserializable'],
];

$manager = new MongoDB\Driver\Manager(URI);
$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));

foreach ($fieldPaths as $fieldPath) {
    $typeMap = ['fieldPaths' => $fieldPath];

    printf("Test typeMap: %s\n", json_encode($typeMap));

    echo throws(function() use ($cursor, $typeMap) {
        $cursor->setTypeMap($typeMap);
    }, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Test typeMap: {"fieldPaths":"notAnArray"}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
The 'fieldPaths' element is not an array

Test typeMap: {"fieldPaths":["notAssociative"]}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
The 'fieldPaths' element is not an associative array

Test typeMap: {"fieldPaths":{"missing":"MissingClass"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist

Test typeMap: {"fieldPaths":{"abstract":"MyAbstractDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyAbstractDocument is not instantiatable

Test typeMap: {"fieldPaths":{"my":"MyDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyDocument does not implement MongoDB\BSON\Unserializable

Test typeMap: {"fieldPaths":{"unserialize":"MongoDB\\BSON\\Unserializable"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MongoDB\BSON\Unserializable is not instantiatable

===DONE===
