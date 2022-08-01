--TEST--
MongoDB\BSON\Binary can be cloned (PHP >= 8.2)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.2'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$types = array(
    MongoDB\BSON\Binary::TYPE_GENERIC,
    MongoDB\BSON\Binary::TYPE_FUNCTION,
    MongoDB\BSON\Binary::TYPE_OLD_BINARY,
    MongoDB\BSON\Binary::TYPE_OLD_UUID,
    MongoDB\BSON\Binary::TYPE_UUID,
    MongoDB\BSON\Binary::TYPE_MD5,
    MongoDB\BSON\Binary::TYPE_USER_DEFINED,
    MongoDB\BSON\Binary::TYPE_USER_DEFINED+5,
);
foreach($types as $type) {
    // Use 16-byte data to satisfy UUID requirements
    $binary = new MongoDB\BSON\Binary('randomBinaryData', $type);

    $clone = clone $binary;

    var_dump($clone == $binary);
    var_dump($clone === $binary);

    unset($binary);

    var_dump($clone);
}
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
object(MongoDB\BSON\Binary)#%d (2) {
  ["data"]=>
  string(16) "randomBinaryData"
  ["type"]=>
  int(0)
}
bool(true)
bool(false)
object(MongoDB\BSON\Binary)#%d (2) {
  ["data"]=>
  string(16) "randomBinaryData"
  ["type"]=>
  int(1)
}
bool(true)
bool(false)
object(MongoDB\BSON\Binary)#%d (2) {
  ["data"]=>
  string(16) "randomBinaryData"
  ["type"]=>
  int(2)
}
bool(true)
bool(false)
object(MongoDB\BSON\Binary)#%d (2) {
  ["data"]=>
  string(16) "randomBinaryData"
  ["type"]=>
  int(3)
}
bool(true)
bool(false)
object(MongoDB\BSON\Binary)#%d (2) {
  ["data"]=>
  string(16) "randomBinaryData"
  ["type"]=>
  int(4)
}
bool(true)
bool(false)
object(MongoDB\BSON\Binary)#%d (2) {
  ["data"]=>
  string(16) "randomBinaryData"
  ["type"]=>
  int(5)
}
bool(true)
bool(false)
object(MongoDB\BSON\Binary)#%d (2) {
  ["data"]=>
  string(16) "randomBinaryData"
  ["type"]=>
  int(128)
}
bool(true)
bool(false)
object(MongoDB\BSON\Binary)#%d (2) {
  ["data"]=>
  string(16) "randomBinaryData"
  ["type"]=>
  int(133)
}
===DONE===
