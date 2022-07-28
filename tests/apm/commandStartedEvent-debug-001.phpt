--TEST--
MongoDB\Driver\Monitoring\CommandStartedEvent debug output
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MySubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event): void
    {
        var_dump($event);
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void
    {
    }
}

$manager = create_test_manager();
$manager->addSubscriber(new MySubscriber);

$manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\Monitoring\CommandStartedEvent)#%d (%d) {
  ["command"]=>
  object(stdClass)#%d (%d) {%A
  }
  ["commandName"]=>
  string(4) "ping"
  ["databaseName"]=>
  string(%d) "%s"
  ["operationId"]=>
  string(%d) "%d"
  ["requestId"]=>
  string(%d) "%d"
  ["server"]=>
  object(MongoDB\Driver\Server)#%d (%d) {%A
  }
  ["serviceId"]=>
  %r(NULL|object\(MongoDB\\BSON\\ObjectId\).*)%r
  ["serverConnectionId"]=>
  %r(NULL|int\(\d+\))%r
}
===DONE===
