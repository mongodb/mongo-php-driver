--TEST--
MongoDB\Driver\ReadPreference construction (invalid mode)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    new MongoDB\Driver\ReadPreference('foo');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Invalid mode: 'foo'
===DONE===
