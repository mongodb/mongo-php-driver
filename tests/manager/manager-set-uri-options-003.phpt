--TEST--
MongoDB\Driver\Manager: SSL options in URI and 'options' don't leak
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_ssl(); ?>
<?php skip_if_not_ssl(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$options = array(
    "pem_pwd" => "does-not-matter",
);

$manager = new MongoDB\Driver\Manager(URI . '&sslclientcertificatekeypassword=does-also-not-matter', [], $options);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: MongoDB\Driver\Manager::__construct(): The "pem_pwd" driver option is deprecated. Please use the "tlsCertificateKeyFilePassword" URI option instead.%s
===DONE===
