--TEST--
MongoDB\BSON\fromPHP(): bsonSerialize() allows arrays, stdClass instances, BSON arrays, and BSON documents
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

class MyDocument implements MongoDB\BSON\Serializable
{
    private $data;

    public function __construct($data = null)
    {
        $this->data = $data;
    }

    #[\ReturnTypeWillChange]
    public function bsonSerialize()
    {
        return $this->data;
    }
}

class MyPersistableDocument extends MyDocument implements MongoDB\BSON\Persistable
{
    public function bsonUnserialize(array $data): void
    {
        $this->data = $data;
    }
}

$tests = array(
    array(1, 2, 3),
    array('foo' => 'bar'),
    (object) array(1, 2, 3),
    (object) array('foo' => 'bar'),
    /* PackedArray cannot be serialized as a root document. Additionally, it
     * will fail return type validation for Persistable::bsonSerialize(). */
    MongoDB\BSON\PackedArray::fromPHP([1, 2, 3]),
    MongoDB\BSON\Document::fromPHP(['foo' => 'bar']),
);

echo "Testing top-level objects\n";

foreach ($tests as $test) {
    try {
        echo toJson(fromPHP(new MyDocument($test))), "\n";
    } catch (Exception $e) {
        printf("%s: %s\n", get_class($e), $e->getMessage());
    }
    try {
        echo toJson(fromPHP(new MyPersistableDocument($test))), "\n";
    } catch (Exception $e) {
        printf("%s: %s\n", get_class($e), $e->getMessage());
    }
}

echo "\nTesting nested objects\n";

foreach ($tests as $test) {
    try {
        echo toJson(fromPHP(new MyDocument(['nested' => new MyDocument($test)]))), "\n";
    } catch (Exception $e) {
        printf("%s: %s\n", get_class($e), $e->getMessage());
    }
    try {
        echo toJson(fromPHP(new MyDocument(['nested' => new MyPersistableDocument($test)]))), "\n";
    } catch (Exception $e) {
        printf("%s: %s\n", get_class($e), $e->getMessage());
    }
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing top-level objects
{ "0" : 1, "1" : 2, "2" : 3 }
{ "__pclass" : { "$binary" : "TXlQZXJzaXN0YWJsZURvY3VtZW50", "$type" : "80" }, "0" : 1, "1" : 2, "2" : 3 }
{ "foo" : "bar" }
{ "__pclass" : { "$binary" : "TXlQZXJzaXN0YWJsZURvY3VtZW50", "$type" : "80" }, "foo" : "bar" }
{ "0" : 1, "1" : 2, "2" : 3 }
{ "__pclass" : { "$binary" : "TXlQZXJzaXN0YWJsZURvY3VtZW50", "$type" : "80" }, "0" : 1, "1" : 2, "2" : 3 }
{ "foo" : "bar" }
{ "__pclass" : { "$binary" : "TXlQZXJzaXN0YWJsZURvY3VtZW50", "$type" : "80" }, "foo" : "bar" }
MongoDB\Driver\Exception\UnexpectedValueException: MongoDB\BSON\PackedArray cannot be serialized as a root document
MongoDB\Driver\Exception\UnexpectedValueException: Expected MyPersistableDocument::bsonSerialize() to return an array, stdClass, or MongoDB\BSON\Document, MongoDB\BSON\PackedArray given
{ "foo" : "bar" }
{ "__pclass" : { "$binary" : "TXlQZXJzaXN0YWJsZURvY3VtZW50", "$type" : "80" }, "foo" : "bar" }

Testing nested objects
{ "nested" : [ 1, 2, 3 ] }
{ "nested" : { "__pclass" : { "$binary" : "TXlQZXJzaXN0YWJsZURvY3VtZW50", "$type" : "80" }, "0" : 1, "1" : 2, "2" : 3 } }
{ "nested" : { "foo" : "bar" } }
{ "nested" : { "__pclass" : { "$binary" : "TXlQZXJzaXN0YWJsZURvY3VtZW50", "$type" : "80" }, "foo" : "bar" } }
{ "nested" : { "0" : 1, "1" : 2, "2" : 3 } }
{ "nested" : { "__pclass" : { "$binary" : "TXlQZXJzaXN0YWJsZURvY3VtZW50", "$type" : "80" }, "0" : 1, "1" : 2, "2" : 3 } }
{ "nested" : { "foo" : "bar" } }
{ "nested" : { "__pclass" : { "$binary" : "TXlQZXJzaXN0YWJsZURvY3VtZW50", "$type" : "80" }, "foo" : "bar" } }
{ "nested" : [ 1, 2, 3 ] }
MongoDB\Driver\Exception\UnexpectedValueException: Expected MyPersistableDocument::bsonSerialize() to return an array, stdClass, or MongoDB\BSON\Document, MongoDB\BSON\PackedArray given
{ "nested" : { "foo" : "bar" } }
{ "nested" : { "__pclass" : { "$binary" : "TXlQZXJzaXN0YWJsZURvY3VtZW50", "$type" : "80" }, "foo" : "bar" } }
===DONE===
