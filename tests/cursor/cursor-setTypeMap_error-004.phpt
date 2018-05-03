--TEST--
Cursor::setTypeMap(): invalid fieldPaths keys
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

class MyDocument extends ArrayObject implements MongoDB\BSON\Unserializable
{
    function bsonUnserialize(array $data)
    {
        parent::__construct($data, ArrayObject::ARRAY_AS_PROPS);
    }
}

$fieldPaths = [
    ['' => 'MyDocument'],
    ['.foo' => 'MyDocument'],
    ['...' => 'MyDocument'],
    ['foo.' => 'MyDocument'],
    ['foo..bar' => 'MyDocument'],
];

$manager = new MongoDB\Driver\Manager(URI);
$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));

foreach ($fieldPaths as $fieldPath) {
    $typeMap = ['fieldPaths' => $fieldPath];

    printf("Test typeMap: %s\n", json_encode($typeMap));

    echo throws(function() use ($cursor, $typeMap) {
        $cursor->setTypeMap($typeMap);
    }, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Test typeMap: {"fieldPaths":{"":"MyDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
The 'fieldPaths' element may not be an empty string

Test typeMap: {"fieldPaths":{".foo":"MyDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
A 'fieldPaths' key may not start with a '.'

Test typeMap: {"fieldPaths":{"...":"MyDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
A 'fieldPaths' key may not start with a '.'

Test typeMap: {"fieldPaths":{"foo.":"MyDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
A 'fieldPaths' key may not end with a '.'

Test typeMap: {"fieldPaths":{"foo..bar":"MyDocument"}}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
A 'fieldPaths' key may not have an empty segment

===DONE===
