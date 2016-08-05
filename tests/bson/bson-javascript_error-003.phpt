--TEST--
MongoDB\BSON\Javascript::__construct() does not allow code to contain null bytes
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\BSON\Javascript("function foo() { return '\0'; }");
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Code cannot contain null bytes
===DONE===
