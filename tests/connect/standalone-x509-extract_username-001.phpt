--TEST--
Connect to MongoDB with SSL and X509 auth and username retrieved from cert
--XFAIL--
X509 tests must be reimplemented (PHPC-1262)
parse_url() tests must be reimplemented (PHPC-1177)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_ssl(['OpenSSL', 'Secure Transport', 'Secure Channel']); ?>
<?php skip_if_not_ssl(); ?>
<?php skip_if_no_ssl_dir(); ?>
<?php skip_if_not_auth_mechanism('MONGODB-X509'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$uriOptions = [
    'authMechanism' => 'MONGODB-X509',
    'tls' => true,
    // libmongoc does not allow the hostname to be overridden as "server"
    'tlsAllowInvalidHostnames' => true,
    'tlsAllowInvalidCertificates' => false,
    'tlsCAFile' => SSL_DIR . '/ca.pem',
    'tlsCertificateKeyFile' => SSL_DIR . '/client.pem',
];

$parsed = parse_url(URI);
$uri = sprintf('mongodb://%s:%d', $parsed['host'], $parsed['port']);

$manager = create_test_manager($uri, $uriOptions);
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
