--TEST--
MongoDB\Driver\Monitoring\CommandFailedEvent
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MySubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event): void
    {
        /* bson_get_monotonic_time() may only have 10-16 millisecond precision
         * on Windows. Sleep to ensure that a non-zero value is reported for
         * CommandFailedEvent's duration. */
        if (PHP_OS_FAMILY === 'Windows') {
            usleep(25000);
        }
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void
    {
        var_dump($event->getCommandName());
        var_dump($event->getDatabaseName());
        var_dump($event->getDurationMicros());
        echo "getDurationMicros() returns > 0: ", $event->getDurationMicros() > 0 ? 'yes' : 'no', "\n";
        var_dump($event->getError() instanceof MongoDB\Driver\Exception\Exception);
        var_dump($event->getOperationId());
        var_dump($event->getReply());
        var_dump($event->getRequestId());
        var_dump($event->getServer());

        /* Note: getServerConnectionId() and getServiceId() have more stringent
         * requirements and are tested separately. */
    }
}

$manager = create_test_manager();

$subscriber = new MySubscriber();
MongoDB\Driver\Monitoring\addSubscriber($subscriber);

$command = new MongoDB\Driver\Command(['unsupportedCommand' => 1]);

try {
    $manager->executeCommand('admin', $command);
} catch (Exception $e) {
}

?>
--EXPECTF--
string(18) "unsupportedCommand"
string(5) "admin"
int(%d)
getDurationMicros() returns > 0: yes
bool(true)
string(%d) "%d"
object(stdClass)#%d (%d) {
  %A
}
string(%d) "%d"
object(MongoDB\Driver\Server)#%d (%d) {
  %A
}
