--TEST--
PHPC-1006: Do not skip __pclass in Serializable::bsonSerialize() return value
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class MyClass implements MongoDB\BSON\Serializable
{
    function bsonSerialize()
    {
        return [
            '__pclass' => 'baz',
            'foo' => 'bar',
        ];
    }
}

hex_dump(fromPHP(new MyClass));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
     0 : 24 00 00 00 02 5f 5f 70 63 6c 61 73 73 00 04 00  [$....__pclass...]
    10 : 00 00 62 61 7a 00 02 66 6f 6f 00 04 00 00 00 62  [..baz..foo.....b]
    20 : 61 72 00 00                                      [ar..]
===DONE===
