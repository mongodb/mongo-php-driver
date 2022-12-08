--TEST--
MongoDB\BSON\Document::fromPHP(): Serializable::bsonSerialize() returns array or stdClass
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

    #[ReturnTypeWillChange]
    public function bsonSerialize()
    {
        return $this->data;
    }
}

$tests = [
    [1, 2, 3],
    ['foo' => 'bar'],
    (object) [1, 2, 3],
    (object) ['foo' => 'bar'],
];

echo "Testing top-level objects\n";

foreach ($tests as $test) {
    try {
        echo MongoDB\BSON\Document::fromPHP(new MyDocument($test))->toRelaxedExtendedJson(), "\n";
    } catch (MongoDB\Driver\Exception\UnexpectedValueException $e) {
        echo $e->getMessage(), "\n";
    }
}

echo "\nTesting nested objects\n";

foreach ($tests as $test) {
    try {
        echo MongoDB\BSON\Document::fromPHP(new MyDocument(['nested' => new MyDocument($test)]))->toRelaxedExtendedJson(), "\n";
    } catch (MongoDB\Driver\Exception\UnexpectedValueException $e) {
        echo $e->getMessage(), "\n";
    }
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing top-level objects
{ "0" : 1, "1" : 2, "2" : 3 }
{ "foo" : "bar" }
{ "0" : 1, "1" : 2, "2" : 3 }
{ "foo" : "bar" }

Testing nested objects
{ "nested" : [ 1, 2, 3 ] }
{ "nested" : { "foo" : "bar" } }
{ "nested" : { "0" : 1, "1" : 2, "2" : 3 } }
{ "nested" : { "foo" : "bar" } }
===DONE===
