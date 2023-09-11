--TEST--
MongoDB\Driver\Logging\log(): Invalid level
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\Driver\Logging\Logger;
use function MongoDB\Driver\Logging\log;

echo throws(function() {
    log(Logger::LEVEL_ERROR - 1, 'LEVEL_ERROR - 1');
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    log(Logger::LEVEL_TRACE + 1, 'LEVEL_TRACE + 1');
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
