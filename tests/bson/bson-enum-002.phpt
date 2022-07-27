--TEST--
Enums implementing Unserializable
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.1.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

enum MyEnum implements MongoDB\BSON\Unserializable
{
    case foo;

    public function bsonUnserialize(array $data): void
    {
        /* Enums do not maintain state, so this method serves no practical
         * purpose other than being required by the interface. Since an
         * implementation is required, we will log a message to assert that BSON
         * decoding always invokes this method. */
        printf("%s called with: %s\n", __METHOD__, json_encode($data));
    }
}

enum MyBackedEnum: string implements MongoDB\BSON\Unserializable
{
    case foo = 'bar';

    public function bsonUnserialize(array $data): void
    {
        printf("%s called with: %s\n", __METHOD__, json_encode($data));
    }
}

$tests = [
    /* There is no practical reason to use an enum as a root document, since it
     * cannot have an "_id" field, but we'll test this anyway since we're only
     * using BSON functions and not round-tripping data through the server. */
    [
        MyEnum::foo,
        ['root' => MyEnum::class],
    ],
    [
        MyBackedEnum::foo,
        ['root' => MyBackedEnum::class],
    ],
    [
        ['myEnum' => MyEnum::foo],
        ['fieldPaths' => ['myEnum' => MyEnum::class]],
    ],
    [
        ['myBackedEnum' => MyBackedEnum::foo],
        ['fieldPaths' => ['myBackedEnum' => MyBackedEnum::class]],
    ],
];

foreach ($tests as $test) {
    [$document, $typeMap] = $test;

    $bson = fromPHP($document);
    echo "Test ", toJSON($bson), "\n";
    hex_dump($bson);
    var_dump(toPHP($bson, $typeMap));
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Test { "name" : "foo" }
     0 : 13 00 00 00 02 6e 61 6d 65 00 04 00 00 00 66 6f  [.....name.....fo]
    10 : 6f 00 00                                         [o..]
MyEnum::bsonUnserialize called with: {"name":"foo"}
enum(MyEnum::foo)

Test { "name" : "foo", "value" : "bar" }
     0 : 22 00 00 00 02 6e 61 6d 65 00 04 00 00 00 66 6f  ["....name.....fo]
    10 : 6f 00 02 76 61 6c 75 65 00 04 00 00 00 62 61 72  [o..value.....bar]
    20 : 00 00                                            [..]
MyBackedEnum::bsonUnserialize called with: {"name":"foo","value":"bar"}
enum(MyBackedEnum::foo)

Test { "myEnum" : { "name" : "foo" } }
     0 : 20 00 00 00 03 6d 79 45 6e 75 6d 00 13 00 00 00  [ ....myEnum.....]
    10 : 02 6e 61 6d 65 00 04 00 00 00 66 6f 6f 00 00 00  [.name.....foo...]
MyEnum::bsonUnserialize called with: {"name":"foo"}
object(stdClass)#%d (%d) {
  ["myEnum"]=>
  enum(MyEnum::foo)
}

Test { "myBackedEnum" : { "name" : "foo", "value" : "bar" } }
     0 : 35 00 00 00 03 6d 79 42 61 63 6b 65 64 45 6e 75  [5....myBackedEnu]
    10 : 6d 00 22 00 00 00 02 6e 61 6d 65 00 04 00 00 00  [m."....name.....]
    20 : 66 6f 6f 00 02 76 61 6c 75 65 00 04 00 00 00 62  [foo..value.....b]
    30 : 61 72 00 00 00                                   [ar...]
MyBackedEnum::bsonUnserialize called with: {"name":"foo","value":"bar"}
object(stdClass)#%d (%d) {
  ["myBackedEnum"]=>
  enum(MyBackedEnum::foo)
}

===DONE===
