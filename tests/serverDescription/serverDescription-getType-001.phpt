--TEST--
MongoDB\Driver\ServerDescription::getType()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$expected_types = array(
    MongoDB\Driver\ServerDescription::TYPE_STANDALONE,
    MongoDB\Driver\ServerDescription::TYPE_MONGOS,
    MongoDB\Driver\ServerDescription::TYPE_RS_PRIMARY
);

function isExpectedType(int $type, array $expected_types) {
    foreach ($expected_types as $expected_type) {
        if ($expected_type == $type) {
            return true;
        }
    }
    return false;
}

$manager = create_test_manager();
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference('primary'));
$type = $server->getServerDescription()->getType();

var_dump(isExpectedType($type, $expected_types));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
===DONE===
