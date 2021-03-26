--TEST--
MongoDB\Driver\Manager::executeCommand() does not inherit read preference
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_no_secondary(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager(URI, ['readPreference' => 'secondary']);

$command = new MongoDB\Driver\Command(['ping' => 1]);
$cursor = $manager->executeCommand(DATABASE_NAME, $command);

echo "is_primary: ", $cursor->getServer()->isPrimary() ? 'true' : 'false', "\n";
echo "is_secondary: ", $cursor->getServer()->isSecondary() ? 'true' : 'false', "\n\n";
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
is_primary: true
is_secondary: false

===DONE===
