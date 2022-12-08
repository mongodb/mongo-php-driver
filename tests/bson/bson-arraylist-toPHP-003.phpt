--TEST--
MongoDB\BSON\ArrayList::toPHP(): Unserializable as root type
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

class MyArray implements MongoDB\BSON\Unserializable
{
    public $data;

    public function bsonUnserialize(array $data): void
    {
        $this->data = $data;
    }
}

$arrayList = MongoDB\BSON\ArrayList::fromPHP([1, 2, 3]);
var_dump($arrayList->toPHP(['root' => MyArray::class]));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MyArray)#%d (%d) {
  ["data"]=>
  array(3) {
    [0]=>
    int(1)
    [1]=>
    int(2)
    [2]=>
    int(3)
  }
}
===DONE===
