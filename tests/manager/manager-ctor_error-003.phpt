--TEST--
MongoDB\Driver\Manager::__construct(): read preference options of the wrong type will throw an InvalidArgumentException exception
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$invalidOptions = array(
    [],
    123,
    2.5,
    0x00,
    null,
    true
);

foreach ($invalidOptions as $invalidOption) {
    echo throws(function() use($invalidOption) {
        $manager = new MongoDB\Driver\Manager(null, ['readPreference' => $invalidOption]);
    }, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
readPreference URI option expected to be string
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
readPreference URI option expected to be string
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
readPreference URI option expected to be string
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
readPreference URI option expected to be string
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
readPreference URI option expected to be string
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
readPreference URI option expected to be string
===DONE===
