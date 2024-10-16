--TEST--
PHPC-720: Do not persist SSL streams to avoid SSL reinitialization errors
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_ssl(); ?>
<?php skip_if_not_ssl(); ?>
<?php skip_if_no_ssl_dir(); ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$uriOptions = [
    // libmongoc does not allow the hostname to be overridden as "server"
    'tlsAllowInvalidHostnames' => true,
    'tlsCAFile' => SSL_DIR . '/ca.pem',
];

$manager = create_test_manager(URI, $uriOptions);
$cursor = $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));
printf("ping: %d\n", $cursor->toArray()[0]->ok);

unset($manager, $cursor);

$manager = create_test_manager(URI, $uriOptions);
$cursor = $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));
printf("ping: %d\n", $cursor->toArray()[0]->ok);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
ping: 1
ping: 1
===DONE===
