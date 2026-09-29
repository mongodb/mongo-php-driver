--TEST--
MongoDB\BSON\Binary::toArray() requires vector subtype
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    (new MongoDB\BSON\Binary(''))->toArray();
}, 'MongoDB\Driver\Exception\LogicException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\LogicException
Expected Binary of type vector (9) but it is 0
===DONE===
