--TEST--
MongoDB\Driver\Manager::__construct(): directConnection=true conflicts with SRV
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    $manager = create_test_manager('mongodb+srv://a.example.com/?directConnection=true');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n\n";

echo throws(function() {
    $manager = create_test_manager('mongodb+srv://a.example.com', ['directConnection' => true]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb+srv://a.example.com/?directConnection=true'. SRV URI not allowed with directConnection option.

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: SRV URI not allowed with directConnection option
===DONE===
