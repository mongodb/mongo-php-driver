--TEST--
Connect to MongoDB with SSL and no host/cert verification (URI options)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_ssl(); ?>
<?php skip_if_not_ssl(); ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$uriOptions = [
    'tlsAllowInvalidHostnames' => true,
    'tlsAllowInvalidCertificates' => true,
];

$manager = create_test_manager(URI, $uriOptions);
$cursor = $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));
printf("ping: %d\n", $cursor->toArray()[0]->ok);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
ping: 1
===DONE===
