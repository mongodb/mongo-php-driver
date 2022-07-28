--TEST--
MongoDB\Driver\Monitoring\CommandFailedEvent debug output
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
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void
    {
        var_dump($event);
    }
}

$manager = create_test_manager();
$manager->addSubscriber(new MySubscriber);

$command = new MongoDB\Driver\Command([
    'aggregate' => COLLECTION_NAME,
    'pipeline' => [['$unsupported' => 1]],
]);

/* Note: Although executeCommand() throws a CommandException, CommandFailedEvent
 * will report a ServerException for its "error" property (PHPC-1990) */
throws(function() use ($manager, $command) {
    $manager->executeCommand(DATABASE_NAME, $command);
}, MongoDB\Driver\Exception\CommandException::class);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\Monitoring\CommandFailedEvent)#%d (%d) {
  ["commandName"]=>
  string(9) "aggregate"
  ["durationMicros"]=>
  int(%d)
  ["error"]=>
  object(MongoDB\Driver\Exception\ServerException)#%d (%d) {%A
  }
  ["operationId"]=>
  string(%d) "%d"
  ["reply"]=>
  object(stdClass)#%d (%d) {%A
  }
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
OK: Got MongoDB\Driver\Exception\CommandException
===DONE===
