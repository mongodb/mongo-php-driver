--TEST--
MongoDB\BSON\fromJSON(): invalid JSON
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

throws(function() {
    MongoDB\BSON\fromJSON('foo');
}, 'MongoDB\Driver\Exception\UnexpectedValueException');

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
===DONE===
