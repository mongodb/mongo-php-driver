--TEST--
MongoDB\Driver\Manager::__construct(): invalid read concern
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\Driver\Manager(null, ['readConcernLevel' => 1]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "readConcernLevel" URI option, 32-bit integer given
===DONE===
