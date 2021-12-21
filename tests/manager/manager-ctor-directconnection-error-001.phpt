--TEST--
MongoDB\Driver\Manager::__construct(): directConnection=true conflicts with multiple seeds
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    $manager = create_test_manager('mongodb://a.example.com,b.example.com/?directConnection=true');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n\n";

echo throws(function() {
    $manager = create_test_manager('mongodb://a.example.com,b.example.com', ['directConnection' => true]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://a.example.com,b.example.com/?directConnection=true'. Multiple seeds not allowed with directConnection option.

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: Multiple seeds not allowed with directConnection option
===DONE===
