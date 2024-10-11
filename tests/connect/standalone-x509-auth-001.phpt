--TEST--
Connect to MongoDB with SSL and X509 auth
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_ssl(); ?>
<?php skip_if_not_ssl(); ?>
<?php skip_if_no_ssl_dir(); ?>
<?php skip_if_not_auth_mechanism('MONGODB-X509'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$uriOptions = [
    // libmongoc does not allow the hostname to be overridden as "server"
    'tlsAllowInvalidHostnames' => true,
    'tlsAllowInvalidCertificates' => false,
    'tlsCAFile' => SSL_DIR . '/ca.pem',
    'tlsCertificateKeyFile' => SSL_DIR . '/client.pem',
];

$manager = create_test_manager(URI, $uriOptions);
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
===DONE===
