--TEST--
MongoDB\Driver\Monitoring\addSubscriber(): Adding a combined CommandSubscriber and LogSubscriber
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\Driver\Command;
use MongoDB\Driver\Monitoring\CommandStartedEvent;
use MongoDB\Driver\Monitoring\CommandSubscriber;
use MongoDB\Driver\Monitoring\LogSubscriber;
use function MongoDB\Driver\Monitoring\addSubscriber;
use function MongoDB\Driver\Monitoring\mongoc_log;
use function MongoDB\Driver\Monitoring\removeSubscriber;

class MySubscriber implements CommandSubscriber, LogSubscriber
{ 
    public function commandStarted(CommandStartedEvent $event): void
    {
        printf("Observed commandStarted for %s\n", $event->getCommandName());
    }
 
    public function commandSucceeded($event): void {}
 
    public function commandFailed($event): void {}

    public function log(int $level, string $domain, string $message): void
    {
        printf("%d: %s: %s\n", $level, $domain, $message);
    }
}

$subscriber = new MySubscriber();
addSubscriber($subscriber);

$manager = create_test_manager();
$manager->executeCommand(DATABASE_NAME, new Command(['ping' => 1]));

mongoc_log(LogSubscriber::LEVEL_DEBUG, 'domain', 'debug');

removeSubscriber($subscriber);

// Neither subscriber implementation should be notified after removal
$manager->executeCommand(DATABASE_NAME, new Command(['ping' => 1]));

mongoc_log(LogSubscriber::LEVEL_DEBUG, 'domain', 'debug');

?>
===DONE===
--EXPECTF--
5: PHONGO: Connection string: '%s'
%a
Observed commandStarted for ping
5: domain: debug
===DONE===
