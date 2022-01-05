--TEST--
Connect to MongoDB with SSL and cert verification error (URI options)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_ssl(); ?>
<?php skip_if_not_ssl(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$uriOptions = [
    // libmongoc does not allow the hostname to be overridden as "server"
    'tlsAllowInvalidHostnames' => true,
    'tlsAllowInvalidCertificates' => false,
];

echo throws(function() use ($uriOptions) {
    $manager = create_test_manager(URI, $uriOptions);
    $cursor = $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));
    var_dump($cursor->toArray()[0]);
}, MongoDB\Driver\Exception\ConnectionException::class, 'executeCommand'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\ConnectionException thrown from executeCommand
%sTLS handshake failed%s
===DONE===
