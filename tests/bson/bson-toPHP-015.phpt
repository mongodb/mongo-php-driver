--TEST--
MongoDB\BSON\toPHP(): Type map values can override Persistable behavior
--DESCRIPTION--
The special type map values below always override Persistable behavior. Note
that an Unserializable class name will only override Persistable behavior if
the class specified in __pclass does not exist or does not implement
MongoDB\BSON\Persistable.
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
    'root document as object' => [
        new MyDocument,
        ['root' => 'object'],
    ],
    'root document as array' => [
        new MyDocument,
        ['root' => 'array'],
    ],
    'root document as bson' => [
        new MyDocument,
        ['root' => 'bson'],
    ],
    'embedded document as object' => [
        ['embedded' => new MyDocument],
        ['document' => 'object'],
    ],
    'embedded document as array' => [
        ['embedded' => new MyDocument],
        ['document' => 'array'],
    ],
    'embedded document as bson' => [
        ['embedded' => new MyDocument],
        ['document' => 'bson'],
    ],
    'fieldPath document as object' => [
        ['embedded' => new MyDocument],
        ['fieldPaths' => ['embedded' => 'object']],
    ],
    'fieldPath document as array' => [
        ['embedded' => new MyDocument],
        ['fieldPaths' => ['embedded' => 'array']],
    ],
    'fieldPath document as bson' => [
        ['embedded' => new MyDocument],
        ['fieldPaths' => ['embedded' => 'bson']],
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
Test root document as object
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
Test root document as array
array(3) {
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
Test root document as bson
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(116) "VQAAAAVfX3BjbGFzcwAKAAAAgE15RG9jdW1lbnQEbGlzdAAaAAAAEDAAAQAAABAxAAIAAAAQMgADAAAAAANtYXAAEgAAAAJmb28ABAAAAGJhcgAAAA=="
  ["value"]=>
  object(MyDocument)#%d (%d) {
    ["data"]=>
    array(2) {
      ["list"]=>
      object(MongoDB\BSON\PackedArray)#%d (%d) {
        ["data"]=>
        string(36) "GgAAABAwAAEAAAAQMQACAAAAEDIAAwAAAAA="
        ["value"]=>
        array(3) {
          [0]=>
          int(1)
          [1]=>
          int(2)
          [2]=>
          int(3)
        }
      }
      ["map"]=>
      object(MongoDB\BSON\Document)#%d (%d) {
        ["data"]=>
        string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
        ["value"]=>
        object(stdClass)#%d (%d) {
          ["foo"]=>
          string(3) "bar"
        }
      }
    }
  }
}
Test embedded document as object
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
Test embedded document as array
object(stdClass)#%d (%d) {
  ["embedded"]=>
  array(3) {
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
    array(1) {
      ["foo"]=>
      string(3) "bar"
    }
  }
}
Test embedded document as bson
object(stdClass)#%d (%d) {
  ["embedded"]=>
  object(MongoDB\BSON\Document)#%d (%d) {
    ["data"]=>
    string(116) "VQAAAAVfX3BjbGFzcwAKAAAAgE15RG9jdW1lbnQEbGlzdAAaAAAAEDAAAQAAABAxAAIAAAAQMgADAAAAAANtYXAAEgAAAAJmb28ABAAAAGJhcgAAAA=="
    ["value"]=>
    object(MyDocument)#%d (%d) {
      ["data"]=>
      array(2) {
        ["list"]=>
        object(MongoDB\BSON\PackedArray)#%d (%d) {
          ["data"]=>
          string(36) "GgAAABAwAAEAAAAQMQACAAAAEDIAAwAAAAA="
          ["value"]=>
          array(3) {
            [0]=>
            int(1)
            [1]=>
            int(2)
            [2]=>
            int(3)
          }
        }
        ["map"]=>
        object(MongoDB\BSON\Document)#%d (%d) {
          ["data"]=>
          string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
          ["value"]=>
          object(stdClass)#%d (%d) {
            ["foo"]=>
            string(3) "bar"
          }
        }
      }
    }
  }
}
Test fieldPath document as object
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
Test fieldPath document as array
object(stdClass)#%d (%d) {
  ["embedded"]=>
  array(3) {
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
Test fieldPath document as bson
object(stdClass)#%d (%d) {
  ["embedded"]=>
  object(MongoDB\BSON\Document)#%d (%d) {
    ["data"]=>
    string(116) "VQAAAAVfX3BjbGFzcwAKAAAAgE15RG9jdW1lbnQEbGlzdAAaAAAAEDAAAQAAABAxAAIAAAAQMgADAAAAAANtYXAAEgAAAAJmb28ABAAAAGJhcgAAAA=="
    ["value"]=>
    object(MyDocument)#%d (%d) {
      ["data"]=>
      array(2) {
        ["list"]=>
        object(MongoDB\BSON\PackedArray)#%d (%d) {
          ["data"]=>
          string(36) "GgAAABAwAAEAAAAQMQACAAAAEDIAAwAAAAA="
          ["value"]=>
          array(3) {
            [0]=>
            int(1)
            [1]=>
            int(2)
            [2]=>
            int(3)
          }
        }
        ["map"]=>
        object(MongoDB\BSON\Document)#%d (%d) {
          ["data"]=>
          string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
          ["value"]=>
          object(stdClass)#%d (%d) {
            ["foo"]=>
            string(3) "bar"
          }
        }
      }
    }
  }
}
===DONE===
