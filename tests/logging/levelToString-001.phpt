--TEST--
MongoDB\Driver\Logging\levelToString()
--FILE--
<?php

use MongoDB\Driver\Logging\Logger;
use function MongoDB\Driver\Logging\levelToString;

var_dump(levelToString(Logger::LEVEL_ERROR));
var_dump(levelToString(Logger::LEVEL_CRITICAL));
var_dump(levelToString(Logger::LEVEL_WARNING));
var_dump(levelToString(Logger::LEVEL_MESSAGE));
var_dump(levelToString(Logger::LEVEL_INFO));
var_dump(levelToString(Logger::LEVEL_DEBUG));
var_dump(levelToString(Logger::LEVEL_TRACE));

// Unsupported log levels
var_dump(levelToString(Logger::LEVEL_ERROR - 1));
var_dump(levelToString(Logger::LEVEL_TRACE + 1));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(5) "ERROR"
string(8) "CRITICAL"
string(7) "WARNING"
string(7) "MESSAGE"
string(4) "INFO"
string(5) "DEBUG"
string(5) "TRACE"
string(7) "UNKNOWN"
string(7) "UNKNOWN"
===DONE===
