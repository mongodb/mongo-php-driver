--TEST--
MongoDB\Driver\TopologyDescription::hasReadableServer()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

class MySubscriber implements MongoDB\Driver\Monitoring\SDAMSubscriber
{
    private $topologyDescription;

    public function topologyChanged(MongoDB\Driver\Monitoring\TopologyChangedEvent $event)
    {
        if (! $this->topologyDescription) {
            $this->topologyDescription = $event->getNewDescription();
            var_dump($this->topologyDescription->hasReadableServer());
        }
    }
}

$subscriber = new MySubscriber;
$manager->addSubscriber($subscriber);

$command = new MongoDB\Driver\Command(['ping' => 1]);
$manager->executeCommand(DATABASE_NAME, $command);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(%s)
===DONE===
