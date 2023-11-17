--TEST--
Connection with auth succeeds
--SKIPIF--
<?php
if (getenv('TESTS') !== 'tests/auth-success.phpt') { echo "skip auth test must be run directly\n"; }
?>
--FILE--
<?php
require_once __DIR__ . "/utils/basic.inc";

$manager = create_test_manager();
$manager->executeQuery(NS, new MongoDB\Driver\Query([]));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===DONE===
