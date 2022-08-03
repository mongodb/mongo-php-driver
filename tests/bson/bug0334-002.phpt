--TEST--
PHPC-334: Encoded BSON should never have multiple __pclass keys
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

class MyClass implements MongoDB\BSON\Persistable {
    #[\ReturnTypeWillChange]
    public function bsonSerialize() {
        return array(
            "__pclass" => "baz",
            "foo" => "bar",
        );
    }
    public function bsonUnserialize(array $data): void {
    }
}

hex_dump(fromPHP(new MyClass))

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
     0 : 28 00 00 00 05 5f 5f 70 63 6c 61 73 73 00 07 00  [(....__pclass...]
    10 : 00 00 80 4d 79 43 6c 61 73 73 02 66 6f 6f 00 04  [...MyClass.foo..]
    20 : 00 00 00 62 61 72 00 00                          [...bar..]
===DONE===
