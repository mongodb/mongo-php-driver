--TEST--
BSON encoding: Object Document Mapper
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
use MongoDB\BSON as BSON;

require_once __DIR__ . "/../utils/basic.inc";
require_once __DIR__ . "/../utils/classes.inc";

$hannes = new Person("Hannes", 42);
$sunnyvale = new Address(94086, "USA");
$kopavogur = new Address(200, "Iceland");
$hannes->addAddress($sunnyvale);
$hannes->addAddress($kopavogur);

$mikola = new Person("Jeremy", 21);
$michigan = new Address(48169, "USA");

$hannes->addFriend($mikola);

var_dump($hannes);

$s = fromArray(array($hannes));
echo "Test ", toJSON($s), "\n";
hex_dump($s);
$ret = toArray($s);
var_dump($ret);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(Person)#%d (5) {
  ["name":protected]=>
  string(6) "Hannes"
  ["age":protected]=>
  int(42)
  ["address":protected]=>
  array(2) {
    [0]=>
    object(Address)#%d (2) {
      ["zip":protected]=>
      int(94086)
      ["country":protected]=>
      string(3) "USA"
    }
    [1]=>
    object(Address)#%d (2) {
      ["zip":protected]=>
      int(200)
      ["country":protected]=>
      string(7) "Iceland"
    }
  }
  ["friends":protected]=>
  array(1) {
    [0]=>
    object(Person)#%d (5) {
      ["name":protected]=>
      string(6) "Jeremy"
      ["age":protected]=>
      int(21)
      ["address":protected]=>
      array(0) {
      }
      ["friends":protected]=>
      array(0) {
      }
      ["secret":protected]=>
      string(24) "Jeremy confidential info"
    }
  }
  ["secret":protected]=>
  string(24) "Hannes confidential info"
}
Test { "0" : { "__pclass" : { "$type" : "80", "$binary" : "UGVyc29u" }, "name" : "Hannes", "age" : 42, "address" : [ { "__pclass" : { "$type" : "80", "$binary" : "QWRkcmVzcw==" }, "zip" : 94086, "country" : "USA" }, { "__pclass" : { "$type" : "80", "$binary" : "QWRkcmVzcw==" }, "zip" : 200, "country" : "Iceland" } ], "friends" : [ { "__pclass" : { "$type" : "80", "$binary" : "UGVyc29u" }, "name" : "Jeremy", "age" : 21, "address" : [  ], "friends" : [  ] } ] } }
     0 : 1f 01 00 00 03 30 00 17 01 00 00 05 5f 5f 70 63  [.....0......__pc]
    10 : 6c 61 73 73 00 06 00 00 00 80 50 65 72 73 6f 6e  [lass......Person]
    20 : 02 6e 61 6d 65 00 07 00 00 00 48 61 6e 6e 65 73  [.name.....Hannes]
    30 : 00 10 61 67 65 00 2a 00 00 00 04 61 64 64 72 65  [..age.*....addre]
    40 : 73 73 00 79 00 00 00 03 30 00 35 00 00 00 05 5f  [ss.y....0.5...._]
    50 : 5f 70 63 6c 61 73 73 00 07 00 00 00 80 41 64 64  [_pclass......Add]
    60 : 72 65 73 73 10 7a 69 70 00 86 6f 01 00 02 63 6f  [ress.zip..o...co]
    70 : 75 6e 74 72 79 00 04 00 00 00 55 53 41 00 00 03  [untry.....USA...]
    80 : 31 00 39 00 00 00 05 5f 5f 70 63 6c 61 73 73 00  [1.9....__pclass.]
    90 : 07 00 00 00 80 41 64 64 72 65 73 73 10 7a 69 70  [.....Address.zip]
    A0 : 00 c8 00 00 00 02 63 6f 75 6e 74 72 79 00 08 00  [......country...]
    B0 : 00 00 49 63 65 6c 61 6e 64 00 00 00 04 66 72 69  [..Iceland....fri]
    C0 : 65 6e 64 73 00 58 00 00 00 03 30 00 50 00 00 00  [ends.X....0.P...]
    D0 : 05 5f 5f 70 63 6c 61 73 73 00 06 00 00 00 80 50  [.__pclass......P]
    E0 : 65 72 73 6f 6e 02 6e 61 6d 65 00 07 00 00 00 4a  [erson.name.....J]
    F0 : 65 72 65 6d 79 00 10 61 67 65 00 15 00 00 00 04  [eremy..age......]
   100 : 61 64 64 72 65 73 73 00 05 00 00 00 00 04 66 72  [address.......fr]
   110 : 69 65 6e 64 73 00 05 00 00 00 00 00 00 00 00     [iends..........]
array(1) {
  [0]=>
  object(Person)#%d (5) {
    ["name":protected]=>
    string(6) "Hannes"
    ["age":protected]=>
    int(42)
    ["address":protected]=>
    array(2) {
      [0]=>
      object(Address)#%d (2) {
        ["zip":protected]=>
        int(94086)
        ["country":protected]=>
        string(3) "USA"
      }
      [1]=>
      object(Address)#%d (2) {
        ["zip":protected]=>
        int(200)
        ["country":protected]=>
        string(7) "Iceland"
      }
    }
    ["friends":protected]=>
    array(1) {
      [0]=>
      object(Person)#%d (5) {
        ["name":protected]=>
        string(6) "Jeremy"
        ["age":protected]=>
        int(21)
        ["address":protected]=>
        array(0) {
        }
        ["friends":protected]=>
        array(0) {
        }
        ["secret":protected]=>
        string(4) "none"
      }
    }
    ["secret":protected]=>
    string(4) "none"
  }
}
===DONE===
