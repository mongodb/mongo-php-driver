--TEST--
MongoDB\Driver\Logging\addLogger(): Adding two loggers
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\Driver\Logging\Logger;
use function MongoDB\Driver\Logging\addLogger;
use function MongoDB\Driver\Logging\log;

class MyLogger implements Logger
{
    private $name;

    public function __construct(string $name)
    {
        $this->name = $name;
    }

    public function log(int $level, string $domain, string $message): void
    {
        printf("%s: %d: %s: %s\n", $this->name, $level, $domain, $message);
    }
}

$logger1 = new MyLogger('ONE');
addLogger($logger1);

log(Logger::LEVEL_ERROR, 'error');
log(Logger::LEVEL_CRITICAL, 'critical');
log(Logger::LEVEL_WARNING, 'warning');

$logger2 = new MyLogger('TWO');
addLogger($logger2);

log(Logger::LEVEL_MESSAGE, 'message');
log(Logger::LEVEL_INFO, 'info');
log(Logger::LEVEL_DEBUG, 'debug');

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
ONE: 0: php: error
ONE: 1: php: critical
ONE: 2: php: warning
ONE: 3: php: message
TWO: 3: php: message
ONE: 4: php: info
TWO: 4: php: info
ONE: 5: php: debug
TWO: 5: php: debug
===DONE===
