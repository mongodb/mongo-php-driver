--TEST--
MongoDB\Driver\Monitoring\TopologyClosedEvent
--SKIPIF--
<?php echo "skip TopologyClosedEvent cannot be observed"; /* TODO: PHPC-2023 */ ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MySubscriber implements MongoDB\Driver\Monitoring\SDAMSubscriber
{
    public function serverChanged(MongoDB\Driver\Monitoring\ServerChangedEvent $event) {}

    public function serverClosed(MongoDB\Driver\Monitoring\ServerClosedEvent $event) {}

    public function serverHeartbeatFailed(MongoDB\Driver\Monitoring\ServerHeartbeatFailedEvent $event) {}

    public function serverHeartbeatStarted(MongoDB\Driver\Monitoring\serverHeartbeatStartedEvent $event) {}

    public function serverHeartbeatSucceeded(MongoDB\Driver\Monitoring\ServerHeartbeatSucceededEvent $event) {}

    public function serverOpening(MongoDB\Driver\Monitoring\ServerOpeningEvent $event) {}
    
    public function topologyChanged(MongoDB\Driver\Monitoring\TopologyChangedEvent $event) {}

    public function topologyClosed(MongoDB\Driver\Monitoring\TopologyClosedEvent $event)
    {
        printf("getTopologyId() returns an ObjectId: %s\n", ($event->getTopologyId() instanceof MongoDB\BSON\ObjectId) ? 'yes' : 'no');

        var_dump($event);
    }

    public function topologyOpening(MongoDB\Driver\Monitoring\TopologyOpeningEvent $event) {}
}

/* Note: using a global subscriber works around the issue of Manager and client
 * unregistration in php_phongo_manager_free_object, but TopologyClosedEvent
 * still cannot be observed due to clearing APM callbacks before
 * mongoc_client_destroy in php_phongo_pclient_destroy (see: PHPC-2023) */
MongoDB\Driver\Monitoring\addSubscriber(new MySubscriber);

/* Note: TopologyChangedEvent can only be observed for non-persistent clients.
 * Persistent clients are destroyed in GSHUTDOWN, long after any PHP objects
 * (including subscribers) are freed. */
$m = create_test_manager(URI, [], ['disableClientPersistence' => true]);

$command = new MongoDB\Driver\Command(['ping' => 1]);
$m->executeCommand(DATABASE_NAME, $command);

unset($m);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
getTopologyId() returns an ObjectId: yes
object(MongoDB\Driver\Monitoring\TopologyClosedEvent)#%d (%d) {
  ["topologyId"]=>
  object(MongoDB\BSON\ObjectId)#%d (%d) {
    ["oid"]=>
    string(24) "%x"
  }
}
===DONE===
