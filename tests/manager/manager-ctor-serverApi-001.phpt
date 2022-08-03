--TEST--
MongoDB\Driver\Manager::__construct(): serverApi driver option
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '5.0'); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

class MySubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event): void
    {
        $command = $event->getCommand();
        var_dump($command->apiVersion);
        var_dump(isset($command->apiStrict));
        var_dump(isset($command->apiDeprecationErrors));
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void
    {
    }
}

$subscriber = new MySubscriber();

$manager = create_test_manager(URI, [], ['serverApi' => new MongoDB\Driver\ServerApi('1')]);

MongoDB\Driver\Monitoring\addSubscriber($subscriber);

$manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(1) "1"
bool(false)
bool(false)
===DONE===
