--TEST--
MongoDB\BSON\toPHP(): Assigning a type map value other than default disables persistable behaviour
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

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

    #[\ReturnTypeWillChange]
    public function bsonSerialize()
    {
        return $this->data;
    }

    public function bsonUnserialize(array $data): void
    {
        foreach (array('list', 'map') as $key) {
            if (isset($data[$key])) {
                $this->data[$key] = $data[$key];
            }
        }
    }
}

$tests = [
    'root document' => [
        new MyDocument,
        ['root' => 'object'],
    ],
    'embedded document' => [
        ['embedded' => new MyDocument],
        ['document' => 'object'],
    ],
    'fieldPath document' => [
        ['embedded' => new MyDocument],
        ['fieldPaths' => ['embedded' => 'object']],
    ],
];

foreach ($tests as $test => [$document, $typeMap]) {
    echo "Test ", $test, "\n";

    $bson = fromPHP($document);
    var_dump(toPHP($bson, $typeMap));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Test root document
object(stdClass)#%d (%d) {
  ["__pclass"]=>
  object(MongoDB\BSON\Binary)#%d (%d) {
    ["data"]=>
    string(10) "MyDocument"
    ["type"]=>
    int(128)
  }
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
  object(stdClass)#%d (%d) {
    ["foo"]=>
    string(3) "bar"
  }
}
Test embedded document
object(stdClass)#%d (%d) {
  ["embedded"]=>
  object(stdClass)#%d (%d) {
    ["__pclass"]=>
    object(MongoDB\BSON\Binary)#%d (%d) {
      ["data"]=>
      string(10) "MyDocument"
      ["type"]=>
      int(128)
    }
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
    object(stdClass)#%d (%d) {
      ["foo"]=>
      string(3) "bar"
    }
  }
}
Test fieldPath document
object(stdClass)#%d (%d) {
  ["embedded"]=>
  object(stdClass)#%d (%d) {
    ["__pclass"]=>
    object(MongoDB\BSON\Binary)#%d (%d) {
      ["data"]=>
      string(10) "MyDocument"
      ["type"]=>
      int(128)
    }
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
    object(stdClass)#%d (%d) {
      ["foo"]=>
      string(3) "bar"
    }
  }
}
===DONE===
