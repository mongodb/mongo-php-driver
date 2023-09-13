--TEST--
MongoDB\Driver\Logging\removeLogger(): Removing one of multiple loggers
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\Driver\Logging\Logger;
use function MongoDB\Driver\Logging\addLogger;
use function MongoDB\Driver\Logging\removeLogger;
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
$logger2 = new MyLogger('TWO');
addLogger($logger1);

log(Logger::LEVEL_ERROR, 'error');
log(Logger::LEVEL_CRITICAL, 'critical');

addLogger($logger2);

log(Logger::LEVEL_WARNING, 'warning');
log(Logger::LEVEL_MESSAGE, 'message');

removeLogger($logger1);

log(Logger::LEVEL_INFO, 'info');
log(Logger::LEVEL_DEBUG, 'debug');

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
ONE: 0: php: error
ONE: 1: php: critical
ONE: 2: php: warning
TWO: 2: php: warning
ONE: 3: php: message
TWO: 3: php: message
TWO: 4: php: info
TWO: 5: php: debug
===DONE===
