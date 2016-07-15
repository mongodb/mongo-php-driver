--TEST--
MongoDB\BSON\Binary::__toString()
--FILE--
<?php

$binary = new MongoDB\BSON\Binary('foobar', MongoDB\BSON\Binary::TYPE_GENERIC);
var_dump((string) $binary);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
string(6) "foobar"
===DONE===
