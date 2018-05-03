--TEST--
MongoDB\Driver\Manager::executeQuery() with filter and projection
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

class MyArrayObject extends ArrayObject implements MongoDB\BSON\Unserializable {
    function bsonUnserialize(array $data) {
        parent::__construct($data);
    }
}
$manager = new MongoDB\Driver\Manager(URI);

$bulk = new \MongoDB\Driver\BulkWrite();
$bulk->insert(array('_id' => 1, array('x' => 2, 'y' => 3)));
$bulk->insert(array('_id' => 2, array('x' => 3, 'y' => 4)));
$bulk->insert(array('_id' => 3, array('x' => 4, 'y' => 5)));
$manager->executeBulkWrite(NS, $bulk);

$query = new MongoDB\Driver\Query(array());
$qr = $manager->executeQuery(NS, $query);
$qr->setTypeMap(array("root"=> "MyArrayObject", "document"=> "MyArrayObject", "array" => "MyArrayObject"));

foreach($qr as $obj) {
    var_dump($obj);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MyArrayObject)#%d (1) {
  [%s]=>
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
}
object(MyArrayObject)#%d (1) {
  [%s]=>
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
}
object(MyArrayObject)#%d (1) {
  [%s]=>
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
}
===DONE===
