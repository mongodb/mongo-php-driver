--TEST--
MongoDB\Driver\Monitoring\CommandFailedEvent
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '3.4'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

class MySubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted( \MongoDB\Driver\Monitoring\CommandStartedEvent $event )
    {
        echo "started: ", $event->getCommandName(), "\n";
    }

    public function commandSucceeded( \MongoDB\Driver\Monitoring\CommandSucceededEvent $event )
    {
      var_dump($event);
    }

    public function commandFailed( \MongoDB\Driver\Monitoring\CommandFailedEvent $event )
    {
        echo "failed: ", $event->getCommandName(), "\n";
        var_dump($event->getReply());
    }
}

$subscriber = new MySubscriber;

MongoDB\Driver\Monitoring\addSubscriber( $subscriber );

$command = new MongoDB\Driver\Command([
  'findAndModify' => COLLECTION_NAME,
  'query' => ['_id' => 'foo'],
  'upsert' => true,
  'new' => true,
]);

try {
    $manager->executeWriteCommand(DATABASE_NAME, $command);
} catch (MongoDB\Driver\Exception\CommandException $e) {}

?>
--EXPECTF--
started: findAndModify
failed: findAndModify
object(stdClass)#%d (%d) {%A
  ["ok"]=>
  float(0)
  ["errmsg"]=>
  string(49) "Either an update or remove=true must be specified"
  ["code"]=>
  int(9)
  ["codeName"]=>
  string(13) "FailedToParse"%A
}
