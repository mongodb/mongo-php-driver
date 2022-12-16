--TEST--
MongoDB\BSON\Document::toPHP(): Null type map values imply default behavior
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

class MyDocument implements MongoDB\BSON\Persistable
{
    public $data;

    public function __construct()
    {
        $this->data = [
            'list' => [1, 2, 3],
            'map' => (object) ['foo' => 'bar'],
        ];
    }

    public function bsonSerialize(): array
    {
        return $this->data;
    }

    public function bsonUnserialize(array $data): void
    {
        foreach (['list', 'map'] as $key) {
            if (isset($data[$key])) {
                $this->data[$key] = $data[$key];
            }
        }
    }
}

$bson = MongoDB\BSON\Document::fromPHP(new MyDocument);
echo "Test ", $bson->toRelaxedExtendedJSON(), "\n";

$typeMap = [
    'array' => null,
    'document' => null,
    'root' => null,
];

var_dump($bson->toPHP($typeMap));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Test { "__pclass" : { "$binary" : { "base64" : "TXlEb2N1bWVudA==", "subType" : "80" } }, "list" : [ 1, 2, 3 ], "map" : { "foo" : "bar" } }
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
