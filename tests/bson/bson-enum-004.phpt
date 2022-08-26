--TEST--
Enums implementing Persistable using PersistableEnum trait
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.1.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

enum MyEnum implements MongoDB\BSON\Persistable
{
    use MongoDB\BSON\PersistableEnum;

    case foo;
}

enum MyBackedEnum: string implements MongoDB\BSON\Persistable
{
    use MongoDB\BSON\PersistableEnum;

    case foo = 'bar';
}

$tests = [
    /* There is no practical reason to use an enum as a root document, since it
     * cannot have an "_id" field, but we'll test this anyway since we're only
     * using BSON functions and not round-tripping data through the server. */
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
Test { "__pclass" : { "$binary" : "TXlFbnVt", "$type" : "80" }, "name" : "foo" }
     0 : 28 00 00 00 05 5f 5f 70 63 6c 61 73 73 00 06 00  [(....__pclass...]
    10 : 00 00 80 4d 79 45 6e 75 6d 02 6e 61 6d 65 00 04  [...MyEnum.name..]
    20 : 00 00 00 66 6f 6f 00 00                          [...foo..]
enum(MyEnum::foo)

Test { "__pclass" : { "$binary" : "TXlCYWNrZWRFbnVt", "$type" : "80" }, "name" : "foo", "value" : "bar" }
     0 : 3d 00 00 00 05 5f 5f 70 63 6c 61 73 73 00 0c 00  [=....__pclass...]
    10 : 00 00 80 4d 79 42 61 63 6b 65 64 45 6e 75 6d 02  [...MyBackedEnum.]
    20 : 6e 61 6d 65 00 04 00 00 00 66 6f 6f 00 02 76 61  [name.....foo..va]
    30 : 6c 75 65 00 04 00 00 00 62 61 72 00 00           [lue.....bar..]
enum(MyBackedEnum::foo)

Test { "myEnum" : { "__pclass" : { "$binary" : "TXlFbnVt", "$type" : "80" }, "name" : "foo" } }
     0 : 35 00 00 00 03 6d 79 45 6e 75 6d 00 28 00 00 00  [5....myEnum.(...]
    10 : 05 5f 5f 70 63 6c 61 73 73 00 06 00 00 00 80 4d  [.__pclass......M]
    20 : 79 45 6e 75 6d 02 6e 61 6d 65 00 04 00 00 00 66  [yEnum.name.....f]
    30 : 6f 6f 00 00 00                                   [oo...]
object(stdClass)#%d (%d) {
  ["myEnum"]=>
  enum(MyEnum::foo)
}

Test { "myBackedEnum" : { "__pclass" : { "$binary" : "TXlCYWNrZWRFbnVt", "$type" : "80" }, "name" : "foo", "value" : "bar" } }
     0 : 50 00 00 00 03 6d 79 42 61 63 6b 65 64 45 6e 75  [P....myBackedEnu]
    10 : 6d 00 3d 00 00 00 05 5f 5f 70 63 6c 61 73 73 00  [m.=....__pclass.]
    20 : 0c 00 00 00 80 4d 79 42 61 63 6b 65 64 45 6e 75  [.....MyBackedEnu]
    30 : 6d 02 6e 61 6d 65 00 04 00 00 00 66 6f 6f 00 02  [m.name.....foo..]
    40 : 76 61 6c 75 65 00 04 00 00 00 62 61 72 00 00 00  [value.....bar...]
object(stdClass)#%d (%d) {
  ["myBackedEnum"]=>
  enum(MyBackedEnum::foo)
}

===DONE===
