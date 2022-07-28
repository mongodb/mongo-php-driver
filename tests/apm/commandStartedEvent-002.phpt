--TEST--
MongoDB\Driver\Monitoring\CommandStartedEvent during mongoc_client_destroy()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";
 
class MySubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{ 
    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event): void
    {
        printf("Observed commandStarted for %s\n", $event->getCommandName());
    }
 
    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void {}
 
    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void {}
}
 
$manager = create_test_manager(URI, [], ['disableClientPersistence' => true]);

$singleSubscriber = new MySubscriber();
$manager->addSubscriber($singleSubscriber);
 
$command = new MongoDB\Driver\Command(['ping' => 1]);
$manager->executeCommand(DATABASE_NAME, $command);

/* Events dispatched during mongoc_client_destroy can only be observed before
 * RSHUTDOWN. This means that we must use a non-persistent client and free it
 * before the script ends. */
unset($manager);

?>
===DONE===
--EXPECT--
Observed commandStarted for ping
Observed commandStarted for endSessions
===DONE===
