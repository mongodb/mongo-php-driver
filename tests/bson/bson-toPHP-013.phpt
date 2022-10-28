--TEST--
Uninstantiatable classes are ignored when processing __pclass
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

abstract class MyAbstractDocument implements MongoDB\BSON\Unserializable {}

class MyDocument {}

trait MyTrait {}

$classes = [
    'MissingClass',
    MyAbstractDocument::class,
    MyDocument::class,
    MyTrait::class,
    MongoDB\BSON\Persistable::class,
];

foreach ($classes as $class) {
    var_dump(toPHP(fromJSON(sprintf('{"x": {"__pclass": {"$binary": "%s", "$type": "80"}, "y": 1}}', base64_encode($class)))));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["x"]=>
  object(stdClass)#%d (%d) {
    ["__pclass"]=>
    object(MongoDB\BSON\Binary)#%d (%d) {
      ["data"]=>
      string(12) "MissingClass"
      ["type"]=>
      int(128)
    }
    ["y"]=>
    int(1)
  }
}
object(stdClass)#%d (%d) {
  ["x"]=>
  object(stdClass)#%d (%d) {
    ["__pclass"]=>
    object(MongoDB\BSON\Binary)#%d (%d) {
      ["data"]=>
      string(18) "MyAbstractDocument"
      ["type"]=>
      int(128)
    }
    ["y"]=>
    int(1)
  }
}
object(stdClass)#%d (%d) {
  ["x"]=>
  object(stdClass)#%d (%d) {
    ["__pclass"]=>
    object(MongoDB\BSON\Binary)#%d (%d) {
      ["data"]=>
      string(10) "MyDocument"
      ["type"]=>
      int(128)
    }
    ["y"]=>
    int(1)
  }
}
object(stdClass)#%d (%d) {
  ["x"]=>
  object(stdClass)#%d (%d) {
    ["__pclass"]=>
    object(MongoDB\BSON\Binary)#%d (%d) {
      ["data"]=>
      string(7) "MyTrait"
      ["type"]=>
      int(128)
    }
    ["y"]=>
    int(1)
  }
}
object(stdClass)#%d (%d) {
  ["x"]=>
  object(stdClass)#%d (%d) {
    ["__pclass"]=>
    object(MongoDB\BSON\Binary)#%d (%d) {
      ["data"]=>
      string(24) "MongoDB\BSON\Persistable"
      ["type"]=>
      int(128)
    }
    ["y"]=>
    int(1)
  }
}
===DONE===
