--TEST--
MongoDB\Driver\Server::executeWriteCommand() write concern inheritance
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_not_enough_data_nodes(2); /* w:2 */ ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";
require_once __DIR__ . "/../utils/observer.php";

$manager = create_test_manager(URI, ['w' => 2, 'wtimeoutms' => 1000]);
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference('primary'));

$command = new MongoDB\Driver\Command([
    'findAndModify' => COLLECTION_NAME,
    'query' => ['x' => 1],
    'upsert' => true,
    'new' => true,
    'update' => ['$inc' => ['x' => 1]],
]);

(new CommandObserver)->observe(
    function() use ($server, $command) {
        $server->executeWriteCommand(DATABASE_NAME, $command);
        $server->executeWriteCommand(DATABASE_NAME, $command, ['writeConcern' => new MongoDB\Driver\WriteConcern(1)]);
    },
    function(stdClass $command) {
        echo json_encode($command->writeConcern), "\n";
    }
);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
{"w":2,"wtimeout":1000}
{"w":1}
===DONE===
