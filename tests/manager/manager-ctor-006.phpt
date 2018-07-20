--TEST--
MongoDB\Driver\Manager::__construct(): Unparsable environmental URI
--ENV--
MONGODB_URI=invalid
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(URI);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'invalid'. Invalid URI Schema, expecting 'mongodb://' or 'mongodb+srv://'.
===DONE===
