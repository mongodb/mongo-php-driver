--TEST--
PHPC-334: Injected __pclass should override a __pclass key in bsonSerialize() return value
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
use MongoDB\BSON as BSON;

require_once __DIR__ . "/../utils/basic.inc";

class MyClass implements BSON\Persistable {
    function bsonSerialize() {
        return array(
            "__pclass" => "baz",
            "foo" => "bar",
        );
    }
    function bsonUnserialize(array $data) {
    }
}

$bson = fromPHP(new MyClass);
$php = toPHP($bson, array('root' => 'array'));

var_dump($php['__pclass']->getData());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(7) "MyClass"
===DONE===
