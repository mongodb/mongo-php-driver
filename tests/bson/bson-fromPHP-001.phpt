--TEST--
MongoDB\BSON\fromPHP(): bsonSerialize() allows array and stdClass
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class MyDocument implements MongoDB\BSON\Serializable
{
    private $data;

    public function __construct($data = null)
    {
        $this->data = $data;
    }

    public function bsonSerialize()
    {
        return $this->data;
    }
}

$tests = array(
    array(1, 2, 3),
    array('foo' => 'bar'),
    (object) array(1, 2, 3),
    (object) array('foo' => 'bar'),
);

echo "Testing top-level objects\n";

foreach ($tests as $test) {
    try {
        echo toJson(fromPHP(new MyDocument($test))), "\n";
    } catch (MongoDB\Driver\Exception\UnexpectedValueException $e) {
        echo $e->getMessage(), "\n";
    }
}

echo "\nTesting nested objects\n";

foreach ($tests as $test) {
    try {
        echo toJson(fromPHP(new MyDocument(array('nested' => new MyDocument($test))))), "\n";
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
