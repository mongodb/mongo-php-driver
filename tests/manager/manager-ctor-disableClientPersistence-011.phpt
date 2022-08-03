--TEST--
MongoDB\Driver\Manager with disableClientPersistence=true referenced by APM subscriber freed in RSHUTDOWN
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MySubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    private $events = [];

    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event): void
    {
        printf("Command started: %s\n", $event->getCommandName());
        $this->events[] = $event;
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void
    {
    }
}

$subscriber = new MySubscriber;


ini_set('mongodb.debug', 'stderr');
$manager = create_test_manager(URI, [], ['disableClientPersistence' => true]);
ini_set('mongodb.debug', '');

MongoDB\Driver\Monitoring\addSubscriber($subscriber);
$command = new MongoDB\Driver\Command(['ping' => 1]);
$manager->executeCommand(DATABASE_NAME, $command);

ini_set('mongodb.debug', 'stderr');

echo "Unsetting manager\n";
unset($manager);

echo "Unsetting subscriber\n";
unset($subscriber);

/* Since the subscriber has not been removed, the remaining internal reference to
 * it will be freed during RSHUTDOWN. */

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%A
[%s]     PHONGO: DEBUG   > Created client with hash: %s
[%s]     PHONGO: DEBUG   > Stored non-persistent client
Command started: ping
Unsetting manager
Unsetting subscriber
===DONE===%A
[%s]     PHONGO: DEBUG   > Destroying non-persistent client for Manager%A
