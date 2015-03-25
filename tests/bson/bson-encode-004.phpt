--TEST--
BSON encoding: Object Document Mapper
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php
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

$s = BSON\fromArray(array($hannes));
echo "Test ", BSON\toJSON($s), "\n";
hex_dump($s);
$ret = BSON\toArray($s);
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
Test { "0" : [ { "$type" : "80", "$binary" : "UGVyc29u" }, "Hannes", 42, [ [ { "$type" : "80", "$binary" : "QWRkcmVzcw==" }, 94086, "USA" ], [ { "$type" : "80", "$binary" : "QWRkcmVzcw==" }, 200, "Iceland" ] ], [ [ { "$type" : "80", "$binary" : "UGVyc29u" }, "Jeremy", 21, {  }, {  } ] ] ] }
     0 : 07 01 00 00 04 30 00 ff 00 00 00 05 5f 5f 00 06  [.....0......__..]
    10 : 00 00 00 80 50 65 72 73 6f 6e 02 6e 61 6d 65 00  [....Person.name.]
    20 : 07 00 00 00 48 61 6e 6e 65 73 00 10 61 67 65 00  [....Hannes..age.]
    30 : 2a 00 00 00 04 61 64 64 72 65 73 73 00 6d 00 00  [*....address.m..]
    40 : 00 04 30 00 2f 00 00 00 05 5f 5f 00 07 00 00 00  [..0./....__.....]
    50 : 80 41 64 64 72 65 73 73 10 7a 69 70 00 86 6f 01  [.Address.zip..o.]
    60 : 00 02 63 6f 75 6e 74 72 79 00 04 00 00 00 55 53  [..country.....US]
    70 : 41 00 00 04 31 00 33 00 00 00 05 5f 5f 00 07 00  [A...1.3....__...]
    80 : 00 00 80 41 64 64 72 65 73 73 10 7a 69 70 00 c8  [...Address.zip..]
    90 : 00 00 00 02 63 6f 75 6e 74 72 79 00 08 00 00 00  [....country.....]
    A0 : 49 63 65 6c 61 6e 64 00 00 00 04 66 72 69 65 6e  [Iceland....frien]
    B0 : 64 73 00 52 00 00 00 04 30 00 4a 00 00 00 05 5f  [ds.R....0.J...._]
    C0 : 5f 00 06 00 00 00 80 50 65 72 73 6f 6e 02 6e 61  [_......Person.na]
    D0 : 6d 65 00 07 00 00 00 4a 65 72 65 6d 79 00 10 61  [me.....Jeremy..a]
    E0 : 67 65 00 15 00 00 00 03 61 64 64 72 65 73 73 00  [ge......address.]
    F0 : 05 00 00 00 00 03 66 72 69 65 6e 64 73 00 05 00  [......friends...]
   100 : 00 00 00 00 00 00 00                             [.......]
array(1) {
  [0]=>
  object(Person)#%d2 (5) {
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
      object(Person)#%d1 (5) {
        ["name":protected]=>
        string(6) "Jeremy"
        ["age":protected]=>
        int(21)
        ["address":protected]=>
        object(stdClass)#%d (0) {
        }
        ["friends":protected]=>
        object(stdClass)#%d0 (0) {
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
