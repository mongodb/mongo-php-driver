--TEST--
MongoDB\BSON\ObjectId constructor type validation
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '7.99'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    new MongoDB\BSON\ObjectId(new stdclass);
}, TypeError::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got TypeError
%SMongoDB\BSON\ObjectId::__construct()%sstring or null, %r(object|stdClass)%r given
===DONE===
