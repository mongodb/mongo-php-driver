--TEST--
MongoDB\Driver\Manager - appname
--FILE--
<?php
$name1 = str_repeat( "PHONGO", 20 );
$name2 = str_repeat( "PHONGO", 20 );
$manager = new \MongoDB\Driver\Manager("mongodb://localhost:27017/?appname={$name1}", [ 'appname' => "2-{$name2}" ] );

$command = new MongoDB\Driver\Command(array('ping' => 1));
$manager->executeCommand("test", $command);
?>
===DONE===
--EXPECT--
===DONE===
