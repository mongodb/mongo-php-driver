--TEST--
MongoDB\Driver\ServerDescription::getTags() with standalone
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_standalone(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference('primary'));

var_dump($server->getServerDescription()->getTags());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(0) {
}
===DONE===
