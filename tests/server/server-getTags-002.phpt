--TEST--
MongoDB\Driver\Server::getTags() with replica set
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_no_secondary(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$tags = $manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY))->getTags();
echo "dc: ", array_key_exists('dc', $tags) ? $tags['dc'] : 'not set', "\n";
echo "ordinal: ", array_key_exists('ordinal', $tags) ? $tags['ordinal'] : 'not set', "\n";

$tags = $manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY))->getTags();
echo "dc: ", array_key_exists('dc', $tags) ? $tags['dc'] : 'not set', "\n";
echo "ordinal: ", array_key_exists('ordinal', $tags) ? $tags['ordinal'] : 'not set', "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
dc: pa
ordinal: one
dc: nyc
ordinal: two
===DONE===
