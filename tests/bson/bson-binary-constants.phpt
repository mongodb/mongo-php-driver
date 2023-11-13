--TEST--
MongoDB\BSON\Binary constants
--FILE--
<?php

use MongoDB\BSON\Binary;

var_dump(Binary::TYPE_GENERIC);
var_dump(Binary::TYPE_FUNCTION);
var_dump(Binary::TYPE_OLD_BINARY);
var_dump(Binary::TYPE_OLD_UUID);
var_dump(Binary::TYPE_UUID);
var_dump(Binary::TYPE_MD5);
var_dump(Binary::TYPE_ENCRYPTED);
var_dump(Binary::TYPE_COLUMN);
var_dump(Binary::TYPE_SENSITIVE);
var_dump(Binary::TYPE_USER_DEFINED);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
int(0)
int(1)
int(2)
int(3)
int(4)
int(5)
int(6)
int(7)
int(8)
int(128)
===DONE===
