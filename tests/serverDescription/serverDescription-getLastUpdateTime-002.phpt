--TEST--
MongoDB\Driver\ServerDescription::getLastUpdateTime() emits warning on truncation of 64-bit value
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php if (4 !== PHP_INT_SIZE) { die('skip Only for 32-bit platform'); } ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference('primary'));

/* The return value of mongoc_server_description_last_update_time() originates
 * from bson_get_monotonic_time(), which has an unspecified starting point and
 * may or may not exceed the range of a 32-bit integer. As such, EXPECTF allows
 * for a possible warning message. Depending on how the value is truncated, it
 * may also be reported as negative. */
var_dump($server->getServerDescription()->getLastUpdateTime());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%r(Warning: Truncating 64-bit value for lastUpdateTime in .+ on line \d+\R)?%rint(%r(-?\d+)%r)
===DONE===
