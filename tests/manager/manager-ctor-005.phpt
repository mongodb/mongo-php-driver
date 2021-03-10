--TEST--
MongoDB\Driver\Manager::__construct(): Ensure environmental URI is parsable
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===DONE===
