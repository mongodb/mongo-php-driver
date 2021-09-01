--TEST--
MongoDB\BSON\Binary serialization (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
--FILE--
<?php

$tests = [
    ['foobar', MongoDB\BSON\Binary::TYPE_GENERIC],
    ['', MongoDB\BSON\Binary::TYPE_GENERIC],
    ["\0foo", MongoDB\BSON\Binary::TYPE_GENERIC],
    [hex2bin('123e4567e89b12d3a456426655440000'), MongoDB\BSON\Binary::TYPE_UUID],
    [md5('foobar', true), MongoDB\BSON\Binary::TYPE_MD5],
];

foreach ($tests as $test) {
    list($data, $type) = $test;

    var_dump($binary = new MongoDB\BSON\Binary($data, $type));
    var_dump($s = serialize($binary));
    var_dump(unserialize($s));
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(6) "foobar"
  ["type"]=>
  int(0)
}
string(70) "O:19:"MongoDB\BSON\Binary":2:{s:4:"data";s:6:"foobar";s:4:"type";i:0;}"
object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(6) "foobar"
  ["type"]=>
  int(0)
}

object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(0) ""
  ["type"]=>
  int(0)
}
string(64) "O:19:"MongoDB\BSON\Binary":2:{s:4:"data";s:0:"";s:4:"type";i:0;}"
object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(0) ""
  ["type"]=>
  int(0)
}

object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(4) "%sfoo"
  ["type"]=>
  int(0)
}
string(68) "O:19:"MongoDB\BSON\Binary":2:{s:4:"data";s:4:"%sfoo";s:4:"type";i:0;}"
object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(4) "%sfoo"
  ["type"]=>
  int(0)
}

object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(16) "%s"
  ["type"]=>
  int(4)
}
string(81) "O:19:"MongoDB\BSON\Binary":2:{s:4:"data";s:16:"%s";s:4:"type";i:4;}"
object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(16) "%s"
  ["type"]=>
  int(4)
}

object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(16) "%s"
  ["type"]=>
  int(5)
}
string(81) "O:19:"MongoDB\BSON\Binary":2:{s:4:"data";s:16:"%s";s:4:"type";i:5;}"
object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(16) "%s"
  ["type"]=>
  int(5)
}

===DONE===
