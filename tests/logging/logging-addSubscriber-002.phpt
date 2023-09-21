--TEST--
MongoDB\Driver\Monitoring\addSubscriber(): Adding two loggers
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\Driver\Monitoring\LogSubscriber;
use function MongoDB\Driver\Monitoring\addSubscriber;
use function MongoDB\Driver\Monitoring\mongoc_log;

class MyLogger implements LogSubscriber
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
addSubscriber($logger1);

mongoc_log(LogSubscriber::LEVEL_ERROR, 'domain', 'error');
mongoc_log(LogSubscriber::LEVEL_CRITICAL, 'domain', 'critical');
mongoc_log(LogSubscriber::LEVEL_WARNING, 'domain', 'warning');

$logger2 = new MyLogger('TWO');
addSubscriber($logger2);

mongoc_log(LogSubscriber::LEVEL_MESSAGE, 'domain', 'message');
mongoc_log(LogSubscriber::LEVEL_INFO, 'domain', 'info');
mongoc_log(LogSubscriber::LEVEL_DEBUG, 'domain', 'debug');

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
ONE: 0: domain: error
ONE: 1: domain: critical
ONE: 2: domain: warning
ONE: 3: domain: message
TWO: 3: domain: message
ONE: 4: domain: info
TWO: 4: domain: info
ONE: 5: domain: debug
TWO: 5: domain: debug
===DONE===
