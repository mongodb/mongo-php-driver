--TEST--
MongoDB\BSON\Binary can be cloned
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

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
    $binary->foo = 'bar';

    $clone = clone $binary;

    var_dump($clone == $binary);
    var_dump($clone === $binary);

    unset($binary);

    var_dump($clone);
    var_dump($clone->foo);
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
string(3) "bar"
bool(true)
bool(false)
object(MongoDB\BSON\Binary)#%d (2) {
  ["data"]=>
  string(16) "randomBinaryData"
  ["type"]=>
  int(1)
}
string(3) "bar"
bool(true)
bool(false)
object(MongoDB\BSON\Binary)#%d (2) {
  ["data"]=>
  string(16) "randomBinaryData"
  ["type"]=>
  int(2)
}
string(3) "bar"
bool(true)
bool(false)
object(MongoDB\BSON\Binary)#%d (2) {
  ["data"]=>
  string(16) "randomBinaryData"
  ["type"]=>
  int(3)
}
string(3) "bar"
bool(true)
bool(false)
object(MongoDB\BSON\Binary)#%d (2) {
  ["data"]=>
  string(16) "randomBinaryData"
  ["type"]=>
  int(4)
}
string(3) "bar"
bool(true)
bool(false)
object(MongoDB\BSON\Binary)#%d (2) {
  ["data"]=>
  string(16) "randomBinaryData"
  ["type"]=>
  int(5)
}
string(3) "bar"
bool(true)
bool(false)
object(MongoDB\BSON\Binary)#%d (2) {
  ["data"]=>
  string(16) "randomBinaryData"
  ["type"]=>
  int(128)
}
string(3) "bar"
bool(true)
bool(false)
object(MongoDB\BSON\Binary)#%d (2) {
  ["data"]=>
  string(16) "randomBinaryData"
  ["type"]=>
  int(133)
}
string(3) "bar"
===DONE===
