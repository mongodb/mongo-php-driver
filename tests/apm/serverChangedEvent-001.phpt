--TEST--
MongoDB\Driver\Monitoring\ServerChangedEvent
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MySubscriber implements MongoDB\Driver\Monitoring\SDAMSubscriber
{
    private $isObserved = false;

    public function serverChanged(MongoDB\Driver\Monitoring\ServerChangedEvent $event): void
    {
        // Ignore multiple events for the purpose of this test
        if ($this->isObserved) {
            return;
        }

        $this->isObserved = true;

        printf("getHost() returns a string: %s\n", is_string($event->getHost()) ? 'yes' : 'no');
        printf("getPort() returns an integer: %s\n", is_integer($event->getPort()) ? 'yes' : 'no');
        printf("getTopologyId() returns an ObjectId: %s\n", ($event->getTopologyId() instanceof MongoDB\BSON\ObjectId) ? 'yes' : 'no');
        printf("getNewDescription() returns a ServerDescription: %s\n", ($event->getNewDescription() instanceof MongoDB\Driver\ServerDescription) ? 'yes' : 'no');
        printf("getPreviousDescription() returns a ServerDescription: %s\n", ($event->getPreviousDescription() instanceof MongoDB\Driver\ServerDescription) ? 'yes' : 'no');

        var_dump($event);
    }

    public function serverClosed(MongoDB\Driver\Monitoring\ServerClosedEvent $event): void {}

    public function serverHeartbeatFailed(MongoDB\Driver\Monitoring\ServerHeartbeatFailedEvent $event): void {}

    public function serverHeartbeatStarted(MongoDB\Driver\Monitoring\serverHeartbeatStartedEvent $event): void {}

    public function serverHeartbeatSucceeded(MongoDB\Driver\Monitoring\ServerHeartbeatSucceededEvent $event): void {}

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
getHost() returns a string: yes
getPort() returns an integer: yes
getTopologyId() returns an ObjectId: yes
getNewDescription() returns a ServerDescription: yes
getPreviousDescription() returns a ServerDescription: yes
object(MongoDB\Driver\Monitoring\ServerChangedEvent)#%d (%d) {
  ["host"]=>
  string(%d) "%s"
  ["port"]=>
  int(%d)
  ["topologyId"]=>
  object(MongoDB\BSON\ObjectId)#%d (%d) {
    ["oid"]=>
    string(24) "%x"
  }
  ["newDescription"]=>
  object(MongoDB\Driver\ServerDescription)#%d (%d) {%A
  }
  ["oldDescription"]=>
  object(MongoDB\Driver\ServerDescription)#%d (%d) {%A
  }
}
===DONE===
