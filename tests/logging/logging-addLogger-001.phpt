--TEST--
MongoDB\Driver\Logging\addLogger(): Adding one logger
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\Driver\Logging\Logger;
use function MongoDB\Driver\Logging\levelToString;
use function MongoDB\Driver\Logging\log;

class MyLogger implements Logger
{
    public string $output = '';

    public function log(int $level, string $domain, string $message): void
    {
        printf("%d: %s: %s\n", $level, $domain, $message);
    }
}

$logger = new MyLogger;
MongoDB\Driver\Logging\addLogger($logger);

log(Logger::LEVEL_ERROR, 'error');
log(Logger::LEVEL_CRITICAL, 'critical');
log(Logger::LEVEL_WARNING, 'warning');
log(Logger::LEVEL_MESSAGE, 'message');
log(Logger::LEVEL_INFO, 'info');
log(Logger::LEVEL_DEBUG, 'debug');
log(Logger::LEVEL_TRACE, 'trace');

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
0: php: error
1: php: critical
2: php: warning
3: php: message
4: php: info
5: php: debug
6: php: trace
===DONE===
