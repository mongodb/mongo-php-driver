--TEST--
MongoDB\Driver\Manager::__construct(): invalid read preference (slaveOk)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://127.0.0.1/?slaveok=other');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager(null, ['slaveOk' => 1]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://127.0.0.1/?slaveok=other'. Unsupported value for "slaveok": "other".
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "slaveOk" URI option, 32-bit integer given
===DONE===
