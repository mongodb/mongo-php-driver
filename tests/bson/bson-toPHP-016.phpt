--TEST--
MongoDB\BSON\toPHP(): Decoding with raw BSON type
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$bson = fromPHP([
    '_id' => 1,
    'object' => [
        'parent1' => [
            'child1' => [ 1, 2, 3 ],
            'child2' => [ 4, 5, 6 ],
        ],
        'parent2' => [
            'child1' => [  7,  8,  9 ],
            'child2' => [ 10, 11, 12 ],
        ],
    ],
]);

$tests = [
    'Root as BSON' => ['root' => 'bson'],
    'Arrays as BSON' => ['array' => 'bson'],
    'Documents as BSON' => ['document' => 'bson'],
    'Field path as BSON' => ['fieldPaths' => ['object.parent1.child1' => 'bson', 'object.parent2' => 'bson']],
    'Non-complex field ignores BSON typemap element' => ['fieldPaths' => ['_id' => 'bson']],
];

foreach ($tests as $name => $typeMap) {
    echo "\n" . $name . "\n";
    var_dump(toPHP($bson, $typeMap));
}
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Root as BSON
object(MongoDB\BSON\BSONDocument)#%d (%d) {
  ["data"]=>
  string(191) "%a"
  ["length"]=>
  int(191)
}

Arrays as BSON
object(stdClass)#%d (%d) {
  ["_id"]=>
  int(1)
  ["object"]=>
  object(stdClass)#%d (%d) {
    ["parent1"]=>
    object(stdClass)#%d (%d) {
      ["child1"]=>
      object(MongoDB\BSON\BSONArray)#%d (%d) {
        ["data"]=>
        string(26) "%a"
        ["length"]=>
        int(26)
      }
      ["child2"]=>
      object(MongoDB\BSON\BSONArray)#%d (%d) {
        ["data"]=>
        string(26) "%a"
        ["length"]=>
        int(26)
      }
    }
    ["parent2"]=>
    object(stdClass)#%d (%d) {
      ["child1"]=>
      object(MongoDB\BSON\BSONArray)#%d (%d) {
        ["data"]=>
        string(26) "%a"
        ["length"]=>
        int(26)
      }
      ["child2"]=>
      object(MongoDB\BSON\BSONArray)#%d (%d) {
        ["data"]=>
        string(26) "%a"
        ["length"]=>
        int(26)
      }
    }
  }
}

Documents as BSON
object(stdClass)#%d (%d) {
  ["_id"]=>
  int(1)
  ["object"]=>
  object(MongoDB\BSON\BSONDocument)#%d (%d) {
    ["data"]=>
    string(169) "%a"
    ["length"]=>
    int(169)
  }
}

Field path as BSON
object(stdClass)#%d (%d) {
  ["_id"]=>
  int(1)
  ["object"]=>
  object(stdClass)#%d (%d) {
    ["parent1"]=>
    object(stdClass)#%d (%d) {
      ["child1"]=>
      object(MongoDB\BSON\BSONArray)#%d (%d) {
        ["data"]=>
        string(26) "%a"
        ["length"]=>
        int(26)
      }
      ["child2"]=>
      array(3) {
        [0]=>
        int(4)
        [1]=>
        int(5)
        [2]=>
        int(6)
      }
    }
    ["parent2"]=>
    object(MongoDB\BSON\BSONDocument)#%d (%d) {
      ["data"]=>
      string(73) "%a"
      ["length"]=>
      int(73)
    }
  }
}

Non-complex field ignores BSON typemap element
object(stdClass)#%d (%d) {
  ["_id"]=>
  int(1)
  ["object"]=>
  object(stdClass)#%d (%d) {
    ["parent1"]=>
    object(stdClass)#%d (%d) {
      ["child1"]=>
      array(3) {
        [0]=>
        int(1)
        [1]=>
        int(2)
        [2]=>
        int(3)
      }
      ["child2"]=>
      array(3) {
        [0]=>
        int(4)
        [1]=>
        int(5)
        [2]=>
        int(6)
      }
    }
    ["parent2"]=>
    object(stdClass)#%d (%d) {
      ["child1"]=>
      array(3) {
        [0]=>
        int(7)
        [1]=>
        int(8)
        [2]=>
        int(9)
      }
      ["child2"]=>
      array(3) {
        [0]=>
        int(10)
        [1]=>
        int(11)
        [2]=>
        int(12)
      }
    }
  }
}
===DONE===
