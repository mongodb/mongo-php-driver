--TEST--
Connect to MongoDB with SSL and no host/cert verification
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('STANDALONE_SSL'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$driverOptions = [
    'allow_invalid_hostname' => true,
    "weak_cert_validation" => true,
];

$manager = new MongoDB\Driver\Manager(STANDALONE_SSL, ['ssl' => true], $driverOptions);
$cursor = $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));
var_dump($cursor->toArray()[0]);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["ok"]=>
  float(1)
}
===DONE===
