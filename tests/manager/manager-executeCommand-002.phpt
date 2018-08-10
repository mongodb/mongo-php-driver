--TEST--
MongoDB\Driver\Manager::executeCommand() takes a read preference in options array
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_no_secondary(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$primary   = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY);
$secondary = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY);

echo "Testing primary:\n";
$command = new MongoDB\Driver\Command(['ping' => 1]);
$cursor = $manager->executeCommand(DATABASE_NAME, $command, ['readPreference' => $primary]);

echo "is_primary: ", $cursor->getServer()->isPrimary() ? 'true' : 'false', "\n";
echo "is_secondary: ", $cursor->getServer()->isSecondary() ? 'true' : 'false', "\n\n";

echo "Testing secondary:\n";
$command = new MongoDB\Driver\Command(['ping' => 1]);
$cursor = $manager->executeCommand(DATABASE_NAME, $command, ['readPreference' => $secondary]);

echo "is_primary: ", $cursor->getServer()->isPrimary() ? 'true' : 'false', "\n";
echo "is_secondary: ", $cursor->getServer()->isSecondary() ? 'true' : 'false', "\n\n";
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing primary:
is_primary: true
is_secondary: false

Testing secondary:
is_primary: false
is_secondary: true

===DONE===
