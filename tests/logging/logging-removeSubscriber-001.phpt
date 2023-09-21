--TEST--
MongoDB\Driver\Monitoring\removeSubscriber(): Removing the only logger
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\Driver\Monitoring\LogSubscriber;
use function MongoDB\Driver\Monitoring\addSubscriber;
use function MongoDB\Driver\Monitoring\mongoc_log;
use function MongoDB\Driver\Monitoring\removeSubscriber;

class MyLogger implements LogSubscriber
{
    public function log(int $level, string $domain, string $message): void
    {
        printf("%d: %s: %s\n", $level, $domain, $message);
    }
}

$logger = new MyLogger;
addSubscriber($logger);

mongoc_log(LogSubscriber::LEVEL_ERROR, 'domain', 'error');
mongoc_log(LogSubscriber::LEVEL_CRITICAL, 'domain', 'critical');
mongoc_log(LogSubscriber::LEVEL_WARNING, 'domain', 'warning');

removeSubscriber($logger);

mongoc_log(LogSubscriber::LEVEL_MESSAGE, 'domain', 'message');
mongoc_log(LogSubscriber::LEVEL_INFO, 'domain', 'info');
mongoc_log(LogSubscriber::LEVEL_DEBUG, 'domain', 'debug');

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
0: domain: error
1: domain: critical
2: domain: warning
===DONE===
