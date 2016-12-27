--TEST--
MongoDB\BSON\fromJSON(): invalid JSON
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    fromJSON('foo');
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Got parse error at 'o', position 1: SPECIAL_EXPECTED
===DONE===
