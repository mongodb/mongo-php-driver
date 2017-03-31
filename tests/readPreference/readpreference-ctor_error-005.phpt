--TEST--
MongoDB\Driver\ReadPreference construction (invalid string mode)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\Driver\ReadPreference("hocuspocus");
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Invalid mode: 'hocuspocus'
===DONE===
