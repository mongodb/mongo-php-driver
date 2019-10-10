--TEST--
Connect to MongoDB with SSL and no host/cert verification
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_ssl(); ?>
<?php skip_if_not_ssl(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$driverOptions = [
    'allow_invalid_hostname' => true,
    "weak_cert_validation" => true,
];

$manager = new MongoDB\Driver\Manager(URI, [], $driverOptions);
$cursor = $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));
var_dump($cursor->toArray()[0]);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: MongoDB\Driver\Manager::__construct(): The "allow_invalid_hostname" driver option is deprecated. Please use the "tlsAllowInvalidHostnames" URI option instead.%s

Deprecated: MongoDB\Driver\Manager::__construct(): The "weak_cert_validation" driver option is deprecated. Please use the "tlsAllowInvalidCertificates" URI option instead.%s
object(stdClass)#%d (%d) {
  ["ok"]=>
  float(1)
}
===DONE===
