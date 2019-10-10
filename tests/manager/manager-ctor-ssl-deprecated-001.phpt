--TEST--
MongoDB\Driver\Manager::__construct(): Test deprecated options
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_ssl(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$deprecatedDriverOptions = [
    ['allow_invalid_hostname' => true],
    ['weak_cert_validation' => true],
    ['allow_self_signed' => true],
    ['pem_file' => 'foo'],
    ['local_cert' => 'foo'],
    ['pem_pwd' => 'foo'],
    ['passphrase' => 'foo'],
    ['ca_file' => 'foo'],
    ['cafile' => 'foo'],
    ['context' => stream_context_create(['ssl' => ['cafile' => 'foo']])],
    ['context' => stream_context_create(['ssl' => ['capath' => 'foo']])],
    ['context' => stream_context_create(['ssl' => ['local_cert' => 'foo']])],
    ['context' => stream_context_create(['ssl' => ['passphrase' => 'foo']])],
    ['context' => stream_context_create(['ssl' => ['allow_self_signed' => true]])],
];

foreach ($deprecatedDriverOptions as $driverOptions) {
    echo raises(
        function () use ($driverOptions) {
            new MongoDB\Driver\Manager('mongodb://127.0.0.1/', [], $driverOptions);
        },
        E_DEPRECATED
    ), "\n";
}

?>
===DONE===
--EXPECT--
OK: Got E_DEPRECATED
MongoDB\Driver\Manager::__construct(): The "allow_invalid_hostname" driver option is deprecated. Please use the "tlsAllowInvalidHostnames" URI option instead.
OK: Got E_DEPRECATED
MongoDB\Driver\Manager::__construct(): The "weak_cert_validation" driver option is deprecated. Please use the "tlsAllowInvalidCertificates" URI option instead.
OK: Got E_DEPRECATED
MongoDB\Driver\Manager::__construct(): The "allow_self_signed" context driver option is deprecated. Please use the "tlsAllowInvalidCertificates" URI option instead.
OK: Got E_DEPRECATED
MongoDB\Driver\Manager::__construct(): The "pem_file" driver option is deprecated. Please use the "tlsCertificateKeyFile" URI option instead.
OK: Got E_DEPRECATED
MongoDB\Driver\Manager::__construct(): The "local_cert" context driver option is deprecated. Please use the "tlsCertificateKeyFile" URI option instead.
OK: Got E_DEPRECATED
MongoDB\Driver\Manager::__construct(): The "pem_pwd" driver option is deprecated. Please use the "tlsCertificateKeyFilePassword" URI option instead.
OK: Got E_DEPRECATED
MongoDB\Driver\Manager::__construct(): The "passphrase" context driver option is deprecated. Please use the "tlsCertificateKeyFilePassword" URI option instead.
OK: Got E_DEPRECATED
MongoDB\Driver\Manager::__construct(): The "ca_file" driver option is deprecated. Please use the "tlsCAFile" URI option instead.
OK: Got E_DEPRECATED
MongoDB\Driver\Manager::__construct(): The "cafile" context driver option is deprecated. Please use the "tlsCAFile" URI option instead.
OK: Got E_DEPRECATED
MongoDB\Driver\Manager::__construct(): The "context" driver option is deprecated.
OK: Got E_DEPRECATED
MongoDB\Driver\Manager::__construct(): The "context" driver option is deprecated.
OK: Got E_DEPRECATED
MongoDB\Driver\Manager::__construct(): The "context" driver option is deprecated.
OK: Got E_DEPRECATED
MongoDB\Driver\Manager::__construct(): The "context" driver option is deprecated.
OK: Got E_DEPRECATED
MongoDB\Driver\Manager::__construct(): The "context" driver option is deprecated.
===DONE===
