--TEST--
PHPC-545: Update does not serialize embedded Persistable's __pclass field
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class Book implements MongoDB\BSON\Persistable
{
    public function bsonSerialize()
    {
        $data = get_object_vars($this);
        return $data;
    }

    public function bsonUnserialize(array $data)
    {
        foreach ($data as $name => $value) {
            $this->{$name} = $value;
        }
    }
}

class Page implements MongoDB\BSON\Persistable
{
    public function bsonSerialize()
    {
        $data = get_object_vars($this);
        return $data;
    }

    public function bsonUnserialize(array $data)
    {
        foreach ($data as $name => $value) {
            $this->{$name} = $value;
        }
    }
}

// Aux
$manager = new MongoDB\Driver\Manager(URI);
$wc = new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY);

// Create
$book = new Book();
$book->title = 'Unnameable';
$book->pages = [];
$page1 = new Page();
$page1->content = 'Lorem ipsum';
$book->pages[] = $page1;
$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert($book);
$result = $manager->executeBulkWrite(NS, $bulk, $wc);
printf("Inserted %d document(s)\n", $result->getInsertedCount());

// Read
$query = new MongoDB\Driver\Query(['title' => $book->title]);
$cursor = $manager->executeQuery(NS, $query);
$bookAfterInsert = $cursor->toArray()[0];

// Update
$bookAfterInsert->description = 'An interesting document';
$page2 = new Page();
$page2->content = 'Dolor sit amet';
$bookAfterInsert->pages[] = $page2;
$bulk = new MongoDB\Driver\BulkWrite;
$bulk->update(['title' => $bookAfterInsert->title], $bookAfterInsert);
$result = $manager->executeBulkWrite(NS, $bulk, $wc);
printf("Modified %d document(s)\n", $result->getModifiedCount());

// Read (again)
$query = new MongoDB\Driver\Query(['title' => $bookAfterInsert->title]);
$cursor = $manager->executeQuery(NS, $query);
$bookAfterUpdate = $cursor->toArray()[0];
var_dump($bookAfterUpdate);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Inserted 1 document(s)
Modified 1 document(s)
object(Book)#%d (%d) {
  ["_id"]=>
  object(MongoDB\BSON\ObjectId)#%d (%d) {
    ["oid"]=>
    string(24) "%s"
  }
  ["__pclass"]=>
  object(MongoDB\BSON\Binary)#%d (%d) {
    ["data"]=>
    string(4) "Book"
    ["type"]=>
    int(%d)
  }
  ["title"]=>
  string(10) "Unnameable"
  ["pages"]=>
  array(2) {
    [0]=>
    object(Page)#%d (%d) {
      ["__pclass"]=>
      object(MongoDB\BSON\Binary)#%d (%d) {
        ["data"]=>
        string(4) "Page"
        ["type"]=>
        int(%d)
      }
      ["content"]=>
      string(11) "Lorem ipsum"
    }
    [1]=>
    object(Page)#%d (%d) {
      ["__pclass"]=>
      object(MongoDB\BSON\Binary)#%d (%d) {
        ["data"]=>
        string(4) "Page"
        ["type"]=>
        int(%d)
      }
      ["content"]=>
      string(14) "Dolor sit amet"
    }
  }
  ["description"]=>
  string(23) "An interesting document"
}
===DONE===
