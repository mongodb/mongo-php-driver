--TEST--
PHPC-924: Cursor::setTypeMap() may unnecessarily convert first BSON document (__pclass)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MyDocument implements MongoDB\BSON\Persistable
{
    private $data;

    public function __construct($id)
    {
        $this->data['_id'] = $id;
    }

    public function bsonSerialize()
    {
        return (object) $this->data;
    }

    public function bsonUnserialize(array $data)
    {
        printf("%s called for ID: %s\n", __METHOD__, $data['_id']);
        $this->data = $data;
    }
}

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(new MyDocument('a'));
$bulk->insert(new MyDocument('b'));
$manager->executeBulkWrite(NS, $bulk);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
/* This type map will have no effect on the query result, since the document
 * only contains an ID, but it allows us to test for unnecessary conversion. */
$cursor->setTypeMap(['array' => 'array']);

foreach ($cursor as $i => $document) {
    var_dump($document);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
MyDocument::bsonUnserialize called for ID: a
object(MyDocument)#%d (%d) {
  ["data":"MyDocument":private]=>
  array(2) {
    ["_id"]=>
    string(1) "a"
    ["__pclass"]=>
    object(MongoDB\BSON\Binary)#%d (%d) {
      ["data"]=>
      string(10) "MyDocument"
      ["type"]=>
      int(128)
    }
  }
}
MyDocument::bsonUnserialize called for ID: b
object(MyDocument)#%d (%d) {
  ["data":"MyDocument":private]=>
  array(2) {
    ["_id"]=>
    string(1) "b"
    ["__pclass"]=>
    object(MongoDB\BSON\Binary)#%d (%d) {
      ["data"]=>
      string(10) "MyDocument"
      ["type"]=>
      int(128)
    }
  }
}
===DONE===
