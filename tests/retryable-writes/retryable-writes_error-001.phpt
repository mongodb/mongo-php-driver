--TEST--
Retryable writes: actionable error message when using retryable writes on unsupported storage engines
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_server_storage_engine('mmapv1'); ?>
<?php skip_if_not_replica_set_or_mongos_with_replica_set(); ?>
<?php skip_if_server_version('<', '3.6'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
$session = $manager->startSession();

echo throws(
    function() use ($manager, $session) {
        $command = new MongoDB\Driver\Command([
            'findAndModify' => COLLECTION_NAME,
            'query' => ['x' => 1],
            'update' => ['$inc' => ['x' => 1]],
        ]);
        $manager->executeReadWriteCommand(DATABASE_NAME, $command, ['session' => $session]);
    },
    \MongoDB\Driver\Exception\CommandException::class
);
echo "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\CommandException
This MongoDB deployment does not support retryable writes. Please add retryWrites=false to your connection string.
===DONE===
