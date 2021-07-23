--TEST--
MongoDB\Driver\ServerDescription::getHelloResponse()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference('primary'));

$helloResponse = $server->getServerDescription()->getHelloResponse();
var_dump(array_key_exists('ismaster', $helloResponse) || array_key_exists('isWritablePrimary', $helloResponse));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
===DONE===
