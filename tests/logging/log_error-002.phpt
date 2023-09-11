--TEST--
MongoDB\Driver\Logging\log(): Message contains null bytes
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\Driver\Logging\Logger;
use function MongoDB\Driver\Logging\log;

echo throws(function() {
    log(Logger::LEVEL_ERROR, "message with null byte\0");
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Log messages cannot contain null bytes. Unexpected null byte after "message with null byte".
===DONE===
