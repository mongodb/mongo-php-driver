--TEST--
Connect to MongoDB with SSL and cert verification (context options)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_ssl(); ?>
<?php skip_if_not_ssl(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$SSL_DIR = realpath(__DIR__ . '/../../scripts/ssl/');

$driverOptions = [
    'context' => stream_context_create([
        'ssl' => [
            // libmongoc does not allow the hostname to be overridden as "server"
            'allow_invalid_hostname' => true,
            'allow_self_signed' => false, // "weak_cert_validation" alias
            'cafile' => $SSL_DIR . '/ca.pem', // "ca_file" alias
        ],
    ]),
];

$manager = new MongoDB\Driver\Manager(URI, [], $driverOptions);
$cursor = $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));
var_dump($cursor->toArray()[0]);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: MongoDB\Driver\Manager::__construct(): The "context" driver option is deprecated.%s

Deprecated: MongoDB\Driver\Manager::__construct(): The "allow_invalid_hostname" driver option is deprecated. Please use the "tlsAllowInvalidHostnames" URI option instead.%s

Deprecated: MongoDB\Driver\Manager::__construct(): The "allow_self_signed" context driver option is deprecated. Please use the "tlsAllowInvalidCertificates" URI option instead.%s

Deprecated: MongoDB\Driver\Manager::__construct(): The "cafile" context driver option is deprecated. Please use the "tlsCAFile" URI option instead.%s
object(stdClass)#%d (%d) {
  ["ok"]=>
  float(1)
}
===DONE===
