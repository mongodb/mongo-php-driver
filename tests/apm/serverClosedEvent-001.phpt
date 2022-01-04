--TEST--
MongoDB\Driver\Monitoring\ServerClosedEvent
--SKIPIF--
<?php echo "skip ServerClosedEvent may not be reliably observed"; /* TODO: PHPC-2023 */ ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MySubscriber implements MongoDB\Driver\Monitoring\SDAMSubscriber
{
    private $isObserved = false;

    public function serverChanged(MongoDB\Driver\Monitoring\ServerChangedEvent $event) {}

    public function serverClosed(MongoDB\Driver\Monitoring\ServerClosedEvent $event)
    {
        // Ignore multiple events for the purpose of this test
        if ($this->isObserved) {
            return;
        }

        $this->isObserved = true;

        printf("getHost() returns a string: %s\n", is_string($event->getHost()) ? 'yes' : 'no');
        printf("getPort() returns an integer: %s\n", is_integer($event->getPort()) ? 'yes' : 'no');
        printf("getTopologyId() returns an ObjectId: %s\n", ($event->getTopologyId() instanceof MongoDB\BSON\ObjectId) ? 'yes' : 'no');

        var_dump($event);
    }

    public function serverHeartbeatFailed(MongoDB\Driver\Monitoring\ServerHeartbeatFailedEvent $event) {}

    public function serverHeartbeatStarted(MongoDB\Driver\Monitoring\serverHeartbeatStartedEvent $event) {}

    public function serverHeartbeatSucceeded(MongoDB\Driver\Monitoring\ServerHeartbeatSucceededEvent $event) {}

    public function serverOpening(MongoDB\Driver\Monitoring\ServerOpeningEvent $event) {}
    
    public function topologyChanged(MongoDB\Driver\Monitoring\TopologyChangedEvent $event) {}

    public function topologyClosed(MongoDB\Driver\Monitoring\TopologyClosedEvent $event) {}

    public function topologyOpening(MongoDB\Driver\Monitoring\TopologyOpeningEvent $event) {}
}

/* Note: using a global subscriber works around the issue of Manager and client
 * unregistration in php_phongo_manager_free_object, but ServerClosedEvent
 * may still not be observed due to clearing APM callbacks before
 * mongoc_client_destroy in php_phongo_pclient_destroy (see: PHPC-2023) */
MongoDB\Driver\Monitoring\addSubscriber(new MySubscriber);

/* Note: ServerClosedEvent may be observed if the host in the URI does not
 * match what is reported in the hello response; however, using a non-persistent
 * client is more reliable since the event can be observed when the Manager and
 * client are freed. */
$m = create_test_manager(URI, [], ['disableClientPersistence' => true]);

$command = new MongoDB\Driver\Command(['ping' => 1]);
$m->executeCommand(DATABASE_NAME, $command);

unset($m);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
getHost() returns a string: yes
getPort() returns an integer: yes
getTopologyId() returns an ObjectId: yes
object(MongoDB\Driver\Monitoring\ServerClosedEvent)#%d (%d) {
  ["host"]=>
  string(%d) "%s"
  ["port"]=>
  int(%d)
  ["topologyId"]=>
  object(MongoDB\BSON\ObjectId)#%d (%d) {
    ["oid"]=>
    string(24) "%x"
  }
}
===DONE===
