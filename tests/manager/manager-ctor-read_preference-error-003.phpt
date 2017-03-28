--TEST--
MongoDB\Driver\Manager::__construct(): invalid read preference (slaveOk)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    $manager = new MongoDB\Driver\Manager('mongodb://127.0.0.1/?slaveok=other');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://127.0.0.1/?slaveok=other'. Unknown option or value for 'slaveok=other'.
===DONE===
