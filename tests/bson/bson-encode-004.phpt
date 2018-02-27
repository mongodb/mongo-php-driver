--TEST--
BSON encoding: Object Document Mapper
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';
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

$s = fromPHP(array($hannes));
echo "Test ", toJSON($s), "\n";
hex_dump($s);
$ret = toPHP($s);
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
  ["addresses":protected]=>
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
      ["addresses":protected]=>
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
Test { "0" : { "__pclass" : { "$binary" : "UGVyc29u", "$type" : "80" }, "name" : "Hannes", "age" : 42, "addresses" : [ { "__pclass" : { "$binary" : "QWRkcmVzcw==", "$type" : "80" }, "zip" : 94086, "country" : "USA" }, { "__pclass" : { "$binary" : "QWRkcmVzcw==", "$type" : "80" }, "zip" : 200, "country" : "Iceland" } ], "friends" : [ { "__pclass" : { "$binary" : "UGVyc29u", "$type" : "80" }, "name" : "Jeremy", "age" : 21, "addresses" : [  ], "friends" : [  ] } ] } }
     0 : 23 01 00 00 03 30 00 1b 01 00 00 05 5f 5f 70 63  [#....0......__pc]
    10 : 6c 61 73 73 00 06 00 00 00 80 50 65 72 73 6f 6e  [lass......Person]
    20 : 02 6e 61 6d 65 00 07 00 00 00 48 61 6e 6e 65 73  [.name.....Hannes]
    30 : 00 10 61 67 65 00 2a 00 00 00 04 61 64 64 72 65  [..age.*....addre]
    40 : 73 73 65 73 00 79 00 00 00 03 30 00 35 00 00 00  [sses.y....0.5...]
    50 : 05 5f 5f 70 63 6c 61 73 73 00 07 00 00 00 80 41  [.__pclass......A]
    60 : 64 64 72 65 73 73 10 7a 69 70 00 86 6f 01 00 02  [ddress.zip..o...]
    70 : 63 6f 75 6e 74 72 79 00 04 00 00 00 55 53 41 00  [country.....USA.]
    80 : 00 03 31 00 39 00 00 00 05 5f 5f 70 63 6c 61 73  [..1.9....__pclas]
    90 : 73 00 07 00 00 00 80 41 64 64 72 65 73 73 10 7a  [s......Address.z]
    A0 : 69 70 00 c8 00 00 00 02 63 6f 75 6e 74 72 79 00  [ip......country.]
    B0 : 08 00 00 00 49 63 65 6c 61 6e 64 00 00 00 04 66  [....Iceland....f]
    C0 : 72 69 65 6e 64 73 00 5a 00 00 00 03 30 00 52 00  [riends.Z....0.R.]
    D0 : 00 00 05 5f 5f 70 63 6c 61 73 73 00 06 00 00 00  [...__pclass.....]
    E0 : 80 50 65 72 73 6f 6e 02 6e 61 6d 65 00 07 00 00  [.Person.name....]
    F0 : 00 4a 65 72 65 6d 79 00 10 61 67 65 00 15 00 00  [.Jeremy..age....]
   100 : 00 04 61 64 64 72 65 73 73 65 73 00 05 00 00 00  [..addresses.....]
   110 : 00 04 66 72 69 65 6e 64 73 00 05 00 00 00 00 00  [..friends.......]
   120 : 00 00 00                                         [...]
object(stdClass)#%d (1) {
  [%r(0|"0")%r]=>
  object(Person)#%d (5) {
    ["name":protected]=>
    string(6) "Hannes"
    ["age":protected]=>
    int(42)
    ["addresses":protected]=>
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
        ["addresses":protected]=>
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
