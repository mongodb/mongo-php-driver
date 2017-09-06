--TEST--
PHPC-1006: Do not modify memory of Persistable::bsonSerialize() return value
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class MyClass implements MongoDB\BSON\Persistable
{
    public $data;

    public function __construct()
    {
        $this->data = [
            '__pclass' => 'baz',
            'foo' => 'bar',
        ];
    }

    function bsonSerialize()
    {
        return $this->data;
    }

    function bsonUnserialize(array $data)
    {
    }
}

$obj = new MyClass;
var_dump($obj->data);
hex_dump(fromPHP($obj));
var_dump($obj->data);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(2) {
  ["__pclass"]=>
  string(3) "baz"
  ["foo"]=>
  string(3) "bar"
}
     0 : 28 00 00 00 05 5f 5f 70 63 6c 61 73 73 00 07 00  [(....__pclass...]
    10 : 00 00 80 4d 79 43 6c 61 73 73 02 66 6f 6f 00 04  [...MyClass.foo..]
    20 : 00 00 00 62 61 72 00 00                          [...bar..]
array(2) {
  ["__pclass"]=>
  string(3) "baz"
  ["foo"]=>
  string(3) "bar"
}
===DONE===
