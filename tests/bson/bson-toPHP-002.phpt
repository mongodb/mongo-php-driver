--TEST--
MongoDB\BSON\fromPHP(): Null type map values imply default behavior
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class MyDocument implements MongoDB\BSON\Persistable
{
    public $data;

    public function __construct()
    {
        $this->data = array(
            'list' => array(1, 2, 3),
            'map' => (object) array('foo' => 'bar'),
        );
    }

    public function bsonSerialize()
    {
        return $this->data;
    }

    public function bsonUnserialize(array $data)
    {
        foreach (array('list', 'map') as $key) {
            if (isset($data[$key])) {
                $this->data[$key] = $data[$key];
            }
        }
    }
}

$bson = fromPHP(new MyDocument);
echo "Test ", toJSON($bson), "\n";
hex_dump($bson);

$typeMap = array(
    'array' => null,
    'document' => null,
    'root' => null,
);

var_dump(toPHP($bson, $typeMap));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Test { "__pclass" : { "$binary" : "TXlEb2N1bWVudA==", "$type" : "80" }, "list" : [ 1, 2, 3 ], "map" : { "foo" : "bar" } }
     0 : 55 00 00 00 05 5f 5f 70 63 6c 61 73 73 00 0a 00  [U....__pclass...]
    10 : 00 00 80 4d 79 44 6f 63 75 6d 65 6e 74 04 6c 69  [...MyDocument.li]
    20 : 73 74 00 1a 00 00 00 10 30 00 01 00 00 00 10 31  [st......0......1]
    30 : 00 02 00 00 00 10 32 00 03 00 00 00 00 03 6d 61  [......2.......ma]
    40 : 70 00 12 00 00 00 02 66 6f 6f 00 04 00 00 00 62  [p......foo.....b]
    50 : 61 72 00 00 00                                   [ar...]
object(MyDocument)#%d (1) {
  ["data"]=>
  array(2) {
    ["list"]=>
    array(3) {
      [0]=>
      int(1)
      [1]=>
      int(2)
      [2]=>
      int(3)
    }
    ["map"]=>
    object(stdClass)#%d (1) {
      ["foo"]=>
      string(3) "bar"
    }
  }
}
===DONE===
