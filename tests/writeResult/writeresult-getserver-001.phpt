--TEST--
MongoDB\Driver\WriteResult::getUpsertedIds()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY));

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['x' => 1]);

$result = $server->executeBulkWrite(NS, $bulk);

var_dump($result->getServer() == $server);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===
