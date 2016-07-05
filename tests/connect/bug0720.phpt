--TEST--
PHPC-720: Do not persist SSL streams to avoid SSL reinitialization errors
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; NEEDS("STANDALONE_SSL"); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$SSL_DIR = realpath(__DIR__ . '/../../scripts/ssl/');

$driverOptions = [
    'peer_name' => 'server',
    'verify_peer' => true,
    'verify_peer_name' => true,
    'allow_self_signed' => false,
    'cafile' => $SSL_DIR . '/ca.pem', /* Defaults to openssl.cafile */
];

$manager = new MongoDB\Driver\Manager(STANDALONE_SSL, ['ssl' => true], $driverOptions);
$cursor = $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));
var_dump($cursor->toArray()[0]);

unset($manager, $cursor);

$manager = new MongoDB\Driver\Manager(STANDALONE_SSL, ['ssl' => true], $driverOptions);
$cursor = $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));
var_dump($cursor->toArray()[0]);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["ok"]=>
  float(1)
}
object(stdClass)#%d (%d) {
  ["ok"]=>
  float(1)
}
===DONE===
