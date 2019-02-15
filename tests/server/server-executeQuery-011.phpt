--TEST--
MongoDB\Driver\Server::executeQuery() sends read preference to mongos
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_mongos(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";
require_once __DIR__ . "/../utils/observer.php";

$manager = new MongoDB\Driver\Manager(URI);
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY));

(new CommandObserver)->observe(
    function() use ($server) {
        $server->executeQuery(
            NS,
            new MongoDB\Driver\Query(['x' => 1]),
            [
                'readPreference' => new \MongoDB\Driver\ReadPreference(\MongoDB\Driver\ReadPreference::RP_NEAREST),
            ]
        );
    },
    function(stdClass $command) {
        echo "Read Preference: ", $command->{'$readPreference'}->mode, "\n";
    }
);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Read Preference: nearest
===DONE===
