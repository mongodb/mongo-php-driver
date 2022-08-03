--TEST--
MongoDB\Driver\Monitoring\TopologyOpeningEvent shares topology ID with other events
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MySubscriber implements MongoDB\Driver\Monitoring\SDAMSubscriber
{
    private $topologyId;
    public $numAssertions = 0;

    public function serverChanged(MongoDB\Driver\Monitoring\ServerChangedEvent $event): void
    {
        $this->assertSameTopologyId($event);
    }

    public function serverClosed(MongoDB\Driver\Monitoring\ServerClosedEvent $event): void
    {
        $this->assertSameTopologyId($event);
    }

    public function serverHeartbeatFailed(MongoDB\Driver\Monitoring\ServerHeartbeatFailedEvent $event): void {}

    public function serverHeartbeatStarted(MongoDB\Driver\Monitoring\serverHeartbeatStartedEvent $event): void {}

    public function serverHeartbeatSucceeded(MongoDB\Driver\Monitoring\ServerHeartbeatSucceededEvent $event): void {}

    public function serverOpening(MongoDB\Driver\Monitoring\ServerOpeningEvent $event): void
    {
        $this->assertSameTopologyId($event);
    }
    
    public function topologyChanged(MongoDB\Driver\Monitoring\TopologyChangedEvent $event): void
    {
        $this->assertSameTopologyId($event);
    }
    
    public function topologyClosed(MongoDB\Driver\Monitoring\TopologyClosedEvent $event): void
    {
        $this->assertSameTopologyId($event);
    }

    public function topologyOpening(MongoDB\Driver\Monitoring\TopologyOpeningEvent $event): void
    {
        $this->topologyId = $event->getTopologyId();
    }

    private function assertSameTopologyId($event)
    {
        $this->numAssertions++;

        if ($event->getTopologyId() != $this->topologyId) {
            throw new UnexpectedValueException(sprintf('Topology ID "%s" from %s does not match "%s" from TopologyOpeningEvent', $event->getTopologyId(), (new ReflectionClass($event))->getShortName(), $this->topologyId));
        }
    }
}

$subscriber = new MySubscriber;

$m = create_test_manager();
$m->addSubscriber($subscriber);

$command = new MongoDB\Driver\Command(['ping' => 1]);
$m->executeCommand(DATABASE_NAME, $command);

/* Rather than expecting a specific number or sequence of events, we can expect
 * that at least one assertion was made and trust that any failures would have
 * resulted in an exception being thrown by assertSameTopologyId(). */
var_dump($subscriber->numAssertions > 1);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===
