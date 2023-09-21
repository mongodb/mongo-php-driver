--TEST--
MongoDB\Driver\Monitoring\addSubscriber(): Trace logs are not reported to loggers
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\Driver\Monitoring\LogSubscriber;
use function MongoDB\Driver\Monitoring\addSubscriber;
use function MongoDB\Driver\Monitoring\mongoc_log;

class MyLogger implements LogSubscriber
{
    public function log(int $level, string $domain, string $message): void
    {
        printf("%d: %s: %s\n", $level, $domain, $message);
    }
}

$logger = new MyLogger;
addSubscriber($logger);

ini_set('mongodb.debug', 'stdout');
mongoc_log(LogSubscriber::LEVEL_DEBUG, 'domain', 'debug1');
// Note: LogSubscriber::LEVEL_TRACE is intentionally undefined
mongoc_log(LogSubscriber::LEVEL_DEBUG + 1, 'domain', 'trace');
mongoc_log(LogSubscriber::LEVEL_DEBUG, 'domain', 'debug2');
ini_set('mongodb.debug', null);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
[%s]     domain: DEBUG   > debug1
5: domain: debug1
[%s]     domain: TRACE   > trace
[%s]     domain: DEBUG   > debug2
5: domain: debug2
===DONE===
