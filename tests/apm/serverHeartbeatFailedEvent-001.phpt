--TEST--
MongoDB\Driver\Monitoring\ServerHeartbeatFailedEvent
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_no_failcommand_failpoint(); ?>
<?php skip_if_not_standalone(); ?>
<?php skip_if_auth(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MySubscriber implements MongoDB\Driver\Monitoring\SDAMSubscriber
{
    public function serverChanged(MongoDB\Driver\Monitoring\ServerChangedEvent $event): void {}

    public function serverClosed(MongoDB\Driver\Monitoring\ServerClosedEvent $event): void {}

    public function serverHeartbeatFailed(MongoDB\Driver\Monitoring\ServerHeartbeatFailedEvent $event): void
    {
        printf("getDurationMicros() returns an integer: %s\n", is_integer($event->getDurationMicros()) ? 'yes' : 'no');
        printf("getError() returns an Exception: %s\n", ($event->getError() instanceof Exception) ? 'yes' : 'no');
        printf("getHost() returns a string: %s\n", is_string($event->getHost()) ? 'yes' : 'no');
        printf("getPort() returns an integer: %s\n", is_integer($event->getPort()) ? 'yes' : 'no');
        printf("isAwaited() returns a bool: %s\n", is_bool($event->isAwaited()) ? 'yes' : 'no');

        var_dump($event);
    }

    public function serverHeartbeatStarted(MongoDB\Driver\Monitoring\serverHeartbeatStartedEvent $event): void {}

    public function serverHeartbeatSucceeded(MongoDB\Driver\Monitoring\ServerHeartbeatSucceededEvent $event): void {}

    public function serverOpening(MongoDB\Driver\Monitoring\ServerOpeningEvent $event): void {}
    
    public function topologyChanged(MongoDB\Driver\Monitoring\TopologyChangedEvent $event): void {}

    public function topologyClosed(MongoDB\Driver\Monitoring\TopologyClosedEvent $event): void {}

    public function topologyOpening(MongoDB\Driver\Monitoring\TopologyOpeningEvent $event): void {}
}

/* Use a separate client to configure the fail point and trigger a heartbeat
 * failure on the second client under test. This test should not be run on
 * load balanced or sharded clusters with multiple mongoses, since we can not
 * reliably select the same target server across two clients. */
$m2 = create_test_manager(URI, [], ['disableClientPersistence' => true]);
configureFailPoint($m2, 'failCommand', ['times' => 1], ['failCommands' => ['isMaster', 'hello'], 'closeConnection' => true]);

$m = create_test_manager(URI, [], ['disableClientPersistence' => true]);
$m->addSubscriber(new MySubscriber);
$command = new MongoDB\Driver\Command(['ping' => 1]);

throws(function() use ($m, $command) {
    $m->executeCommand(DATABASE_NAME, $command);
}, MongoDB\Driver\Exception\ConnectionTimeoutException::class);

configureFailPoint($m2, 'failCommand', 'off');

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
getDurationMicros() returns an integer: yes
getError() returns an Exception: yes
getHost() returns a string: yes
getPort() returns an integer: yes
isAwaited() returns a bool: yes
object(MongoDB\Driver\Monitoring\ServerHeartbeatFailedEvent)#%d (%d) {
  ["host"]=>
  string(%d) "%s"
  ["port"]=>
  int(%d)
  ["awaited"]=>
  bool(%s)
  ["durationMicros"]=>
  int(%d)
  ["error"]=>
  object(MongoDB\Driver\Exception\RuntimeException)#%d (%d) {%A
  }
}
OK: Got MongoDB\Driver\Exception\ConnectionTimeoutException
===DONE===
