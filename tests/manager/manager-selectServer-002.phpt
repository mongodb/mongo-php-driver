--TEST--
MongoDB\Driver\Manager::selectServer() defaults to primary read preference
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php

use MongoDB\Driver\Server;

require_once __DIR__ . "/../utils/basic.inc";

/* Specifying a client-level read preference will also ensure that it is not
 * inherited by php_phongo_manager_select_server. */
$manager = create_test_manager(null, ['readPreference' => 'secondary']);

function isPrimary(Server $server): bool {
    return in_array($server->getType(), [Server::TYPE_STANDALONE, Server::TYPE_MONGOS, Server::TYPE_RS_PRIMARY, Server::TYPE_LOAD_BALANCER]);
}

var_dump(isPrimary($manager->selectServer()));
var_dump(isPrimary($manager->selectServer(null)));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
===DONE===
