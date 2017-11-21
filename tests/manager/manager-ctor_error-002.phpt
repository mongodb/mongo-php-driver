--TEST--
MongoDB\Driver\Manager::__construct(): invalid URI
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    $manager = new MongoDB\Driver\Manager("not a valid connection string");
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'not a valid connection string'. Invalid URI Schema, expecting 'mongodb://' or 'mongodb+srv://'.
===DONE===
