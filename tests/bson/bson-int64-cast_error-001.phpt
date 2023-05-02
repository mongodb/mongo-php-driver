--TEST--
MongoDB\BSON\Int64 casting errors (PHP 8)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$int64 = new MongoDB\BSON\Int64(2);

echo raises(function () use ($int64) {
    var_dump((float) $int64);
}, E_WARNING), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got E_WARNING
Object of class MongoDB\BSON\Int64 could not be converted to float
===DONE===
