--TEST--
PHPC-671: Segfault if Manager is already freed when using selected Server
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY));

unset($manager);

$cursor = $server->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));
var_dump($cursor->toArray()[0]);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["ok"]=>
  float(1)%A
}
===DONE===
