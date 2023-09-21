--TEST--
MongoDB\Driver\mongoc_log(LogSubscriber(): Removing an unregistered logger is a NOP
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\Driver\Monitoring\LogSubscriber;
use function MongoDB\Driver\Monitoring\addSubscriber;
use function MongoDB\Driver\Monitoring\mongoc_log;
use function MongoDB\Driver\Monitoring\removeSubscriber;

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
$logger2 = new MyLogger('TWO');
addSubscriber($logger1);

mongoc_log(LogSubscriber::LEVEL_ERROR, 'domain', 'error');
mongoc_log(LogSubscriber::LEVEL_CRITICAL, 'domain', 'critical');

removeSubscriber($logger2);

mongoc_log(LogSubscriber::LEVEL_WARNING, 'domain', 'warning');
mongoc_log(LogSubscriber::LEVEL_MESSAGE, 'domain', 'message');

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
ONE: 0: domain: error
ONE: 1: domain: critical
ONE: 2: domain: warning
ONE: 3: domain: message
===DONE===
