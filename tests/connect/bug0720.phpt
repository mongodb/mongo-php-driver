--TEST--
PHPC-720: Do not persist SSL streams to avoid SSL reinitialization errors
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_ssl(); ?>
<?php skip_if_not_ssl(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$SSL_DIR = realpath(__DIR__ . '/../../scripts/ssl/');

$driverOptions = [
    // libmongoc does not allow the hostname to be overridden as "server"
    'allow_invalid_hostname' => true,
    'ca_file' => $SSL_DIR . '/ca.pem',
];

$manager = new MongoDB\Driver\Manager(URI, [], $driverOptions);
$cursor = $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));
var_dump($cursor->toArray()[0]);

unset($manager, $cursor);

$manager = new MongoDB\Driver\Manager(URI, [], $driverOptions);
$cursor = $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));
var_dump($cursor->toArray()[0]);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: MongoDB\Driver\Manager::__construct(): The "allow_invalid_hostname" driver option is deprecated. Please use the "tlsAllowInvalidHostnames" URI option instead.%s

Deprecated: MongoDB\Driver\Manager::__construct(): The "ca_file" driver option is deprecated. Please use the "tlsCAFile" URI option instead.%s
object(stdClass)#%d (%d) {
  ["ok"]=>
  float(1)
}
object(stdClass)#%d (%d) {
  ["ok"]=>
  float(1)
}
===DONE===
