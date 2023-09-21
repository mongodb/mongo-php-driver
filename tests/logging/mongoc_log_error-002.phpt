--TEST--
MongoDB\Driver\Monitoring\mongoc_log(): Domain contains null bytes
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\Driver\Monitoring\LogSubscriber;
use function MongoDB\Driver\Monitoring\mongoc_log;

echo throws(function() {
    mongoc_log(LogSubscriber::LEVEL_ERROR, "domain\0", 'message');
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Domain cannot contain null bytes. Unexpected null byte after "domain".
===DONE===
