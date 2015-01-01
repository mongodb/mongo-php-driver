--TEST--
MongoDB\Driver\Server::executeQuery() with filter and projection
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

class MyArrayObject extends ArrayObject implements BSON\Unserializable {
    function bsonUnserialize(array $data) {
        parent::__construct($data);
    }
}
$manager = new MongoDB\Driver\Manager(MONGODB_URI);

$batch = new \MongoDB\Driver\WriteBatch();
$batch->insert(array('_id' => 1, array('x' => 2, 'y' => 3)));
$batch->insert(array('_id' => 2, array('x' => 3, 'y' => 4)));
$batch->insert(array('_id' => 3, array('x' => 4, 'y' => 5)));
$manager->executeWriteBatch(NS, $batch);

$query = new MongoDB\Driver\Query(array());
$qr = $manager->executeQuery(NS, $query);
$qr->setTypemap(array("document"=> "MyArrayObject", "array" => "MyArrayObject"));

foreach($qr as $obj) {
    var_dump($obj);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(2) {
  ["_id"]=>
  int(1)
  [0]=>
  object(MyArrayObject)#%d (1) {
    [%s]=>
    array(2) {
      ["x"]=>
      int(2)
      ["y"]=>
      int(3)
    }
  }
}
array(2) {
  ["_id"]=>
  int(2)
  [0]=>
  object(MyArrayObject)#%d (1) {
    [%s]=>
    array(2) {
      ["x"]=>
      int(3)
      ["y"]=>
      int(4)
    }
  }
}
array(2) {
  ["_id"]=>
  int(3)
  [0]=>
  object(MyArrayObject)#%d (1) {
    [%s]=>
    array(2) {
      ["x"]=>
      int(4)
      ["y"]=>
      int(5)
    }
  }
}
===DONE===
