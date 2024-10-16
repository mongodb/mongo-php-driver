--TEST--
MongoDB\Driver\Manager: SSL options in URI and 'options' don't leak
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_ssl(); ?>
<?php skip_if_not_ssl(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager(
    URI . '&tlsCertificateKeyFilePassword=does-not-matter',
    ['tlsCertificateKeyFilePassword' => 'also-does-not-matter'],
);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===DONE===
