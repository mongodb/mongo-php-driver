--TEST--
MongoDB\Driver\Monitoring\mongoc_log(): Invalid level
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\Driver\Monitoring\LogSubscriber;
use function MongoDB\Driver\Monitoring\mongoc_log;

echo throws(function() {
    mongoc_log(LogSubscriber::LEVEL_ERROR - 1, 'domain', 'message');
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    // Note: LogSubscriber::LEVEL_TRACE is intentionally undefined
    mongoc_log(LogSubscriber::LEVEL_DEBUG + 2, 'domain', 'message');
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected level to be >= 0 and <= 6, -1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected level to be >= 0 and <= 6, 7 given
===DONE===
