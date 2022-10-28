--TEST--
Cursor::setTypeMap(): fieldPaths must be an array, with single key/string elements
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$typeMaps = [
    ['fieldPaths' => 'notAnArray'],
    ['fieldPaths' => ['notAssociative']],
];

$manager = create_test_manager();
$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));

foreach ($typeMaps as $typeMap) {
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

===DONE===
