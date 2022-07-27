--TEST--
Enums serialize as documents and are not unserialized by default
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.1.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

enum MyEnum
{
    case foo;
}

enum MyBackedEnum: string
{
    case foo = 'bar';
}

$tests = [
    MyEnum::foo,
    MyBackedEnum::foo,
    ['myEnum' => MyEnum::foo],
    ['myBackedEnum' => MyBackedEnum::foo],
];

foreach ($tests as $document) {
    $bson = fromPHP($document);
    echo "Test ", toJSON($bson), "\n";
    hex_dump($bson);
    var_dump(toPHP($bson));
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Test { "name" : "foo" }
     0 : 13 00 00 00 02 6e 61 6d 65 00 04 00 00 00 66 6f  [.....name.....fo]
    10 : 6f 00 00                                         [o..]
object(stdClass)#%d (%d) {
  ["name"]=>
  string(3) "foo"
}

Test { "name" : "foo", "value" : "bar" }
     0 : 22 00 00 00 02 6e 61 6d 65 00 04 00 00 00 66 6f  ["....name.....fo]
    10 : 6f 00 02 76 61 6c 75 65 00 04 00 00 00 62 61 72  [o..value.....bar]
    20 : 00 00                                            [..]
object(stdClass)#%d (%d) {
  ["name"]=>
  string(3) "foo"
  ["value"]=>
  string(3) "bar"
}

Test { "myEnum" : { "name" : "foo" } }
     0 : 20 00 00 00 03 6d 79 45 6e 75 6d 00 13 00 00 00  [ ....myEnum.....]
    10 : 02 6e 61 6d 65 00 04 00 00 00 66 6f 6f 00 00 00  [.name.....foo...]
object(stdClass)#%d (%d) {
  ["myEnum"]=>
  object(stdClass)#%d (%d) {
    ["name"]=>
    string(3) "foo"
  }
}

Test { "myBackedEnum" : { "name" : "foo", "value" : "bar" } }
     0 : 35 00 00 00 03 6d 79 42 61 63 6b 65 64 45 6e 75  [5....myBackedEnu]
    10 : 6d 00 22 00 00 00 02 6e 61 6d 65 00 04 00 00 00  [m."....name.....]
    20 : 66 6f 6f 00 02 76 61 6c 75 65 00 04 00 00 00 62  [foo..value.....b]
    30 : 61 72 00 00 00                                   [ar...]
object(stdClass)#%d (%d) {
  ["myBackedEnum"]=>
  object(stdClass)#%d (%d) {
    ["name"]=>
    string(3) "foo"
    ["value"]=>
    string(3) "bar"
  }
}

===DONE===
