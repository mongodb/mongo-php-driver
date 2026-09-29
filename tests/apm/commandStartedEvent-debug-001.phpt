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
  ["host"]=>
  string(%d) "%s"
  ["port"]=>
  int(%d)
  ["commandName"]=>
  string(4) "ping"
  ["databaseName"]=>
  string(%d) "%s"
  ["command"]=>
  object(stdClass)#%d (%d) {%A
  }
  ["operationId"]=>
  string(%d) "%d"
  ["requestId"]=>
  string(%d) "%d"
  ["serviceId"]=>
  %r(NULL|object\(MongoDB\\BSON\\ObjectId\).*)%r
  ["serverConnectionId"]=>
  %r(NULL|int\(\d+\))%r
}
===DONE===
