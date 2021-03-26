--TEST--
MongoDB\BSON\fromJSON(): invalid JSON
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

throws(function() {
    fromJSON('foo');
}, 'MongoDB\Driver\Exception\UnexpectedValueException');

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
===DONE===
