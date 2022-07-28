--TEST--
PHPC-334: Injected __pclass should override a __pclass key in bsonSerialize() return value
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

$bson = fromPHP(new MyClass);
$php = toPHP($bson, array('root' => 'array'));

var_dump($php['__pclass']->getData());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(7) "MyClass"
===DONE===
