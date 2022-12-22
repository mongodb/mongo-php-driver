--TEST--
PHPC-1239: Passing SSL driverOptions overrides SSL options from URI
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager(URI);

$manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===DONE===
