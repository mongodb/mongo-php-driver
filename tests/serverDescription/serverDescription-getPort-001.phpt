--TEST--
MongoDB\Driver\ServerDescription::getPort()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference('primary'));

var_dump($server->getServerDescription()->getPort());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
int(%d)
===DONE===
