--TEST--
MongoDB\Driver\Monitoring\ServerHeartbeatSucceededEvent
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MySubscriber implements MongoDB\Driver\Monitoring\SDAMSubscriber
{
    private $isObserved = false;

    public function serverChanged(MongoDB\Driver\Monitoring\ServerChangedEvent $event): void {}

    public function serverClosed(MongoDB\Driver\Monitoring\ServerClosedEvent $event): void {}

    public function serverHeartbeatFailed(MongoDB\Driver\Monitoring\ServerHeartbeatFailedEvent $event): void {}

    public function serverHeartbeatStarted(MongoDB\Driver\Monitoring\serverHeartbeatStartedEvent $event): void {}

    public function serverHeartbeatSucceeded(MongoDB\Driver\Monitoring\ServerHeartbeatSucceededEvent $event): void
    {
        // Ignore multiple events for the purpose of this test
        if ($this->isObserved) {
            return;
        }

        $this->isObserved = true;

        printf("getDurationMicros() returns an integer: %s\n", is_integer($event->getDurationMicros()) ? 'yes' : 'no');
        printf("getHost() returns a string: %s\n", is_string($event->getHost()) ? 'yes' : 'no');
        printf("getPort() returns an integer: %s\n", is_integer($event->getPort()) ? 'yes' : 'no');
        printf("getReply() returns an object: %s\n", is_object($event->getReply()) ? 'yes' : 'no');
        printf("isAwaited() returns a bool: %s\n", is_bool($event->isAwaited()) ? 'yes' : 'no');

        var_dump($event);
    }

    public function serverOpening(MongoDB\Driver\Monitoring\ServerOpeningEvent $event): void {}
    
    public function topologyChanged(MongoDB\Driver\Monitoring\TopologyChangedEvent $event): void {}

    public function topologyClosed(MongoDB\Driver\Monitoring\TopologyClosedEvent $event): void {}

    public function topologyOpening(MongoDB\Driver\Monitoring\TopologyOpeningEvent $event): void {}
}

$m = create_test_manager();
$m->addSubscriber(new MySubscriber);

$command = new MongoDB\Driver\Command(['ping' => 1]);
$m->executeCommand(DATABASE_NAME, $command);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
getDurationMicros() returns an integer: yes
getHost() returns a string: yes
getPort() returns an integer: yes
getReply() returns an object: yes
isAwaited() returns a bool: yes
object(MongoDB\Driver\Monitoring\ServerHeartbeatSucceededEvent)#%d (%d) {
  ["host"]=>
  string(%d) "%s"
  ["port"]=>
  int(%d)
  ["awaited"]=>
  bool(%s)
  ["reply"]=>
  object(stdClass)#%d (%d) {%A
  }
}
===DONE===
