--TEST--
MongoDB\Driver\Manager::executeWriteCommand() throws CommandException for invalid writeConcern
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_server_version('<', '3.2'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$command = new MongoDB\Driver\Command([
    'findAndModify' => COLLECTION_NAME,
    'query' => ['_id' => 'foo'],
    'update' => ['foo' => ['bar']],
    'upsert' => true,
    'new' => true,
]);

try {
    $manager->executeWriteCommand(DATABASE_NAME, $command, ['writeConcern' => new MongoDB\Driver\WriteConcern("undefined")]);
} catch (MongoDB\Driver\Exception\CommandException $e) {
    printf("%s(%d): %s\n", get_class($e), $e->getCode(), $e->getMessage());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
MongoDB\Driver\Exception\CommandException(79): Write Concern error: No write concern mode named 'undefined' found in replica set configuration
===DONE===
