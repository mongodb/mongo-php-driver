--TEST--
MongoDB\Driver\Manager::addSubscriber() does not support LogSubscriber instances
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

use MongoDB\Driver\Monitoring\CommandSubscriber;
use MongoDB\Driver\Monitoring\LogSubscriber;

class MyLogger implements LogSubscriber
{
    public function log($level, $domain, $message): void {}
}

class MySubscriber implements CommandSubscriber, LogSubscriber
{ 
    public function commandStarted($event): void {}
 
    public function commandSucceeded($event): void {}
 
    public function commandFailed($event): void {}

    public function log($level, $domain, $message): void {}
}

$manager = create_test_manager();

echo throws(function () use ($manager) {
    $manager->addSubscriber(new MyLogger);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function () use ($manager) {
    $manager->addSubscriber(new MySubscriber);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
LogSubscriber instances cannot be registered with a Manager
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
LogSubscriber instances cannot be registered with a Manager
===DONE===
