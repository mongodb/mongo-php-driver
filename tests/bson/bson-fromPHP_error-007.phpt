--TEST--
MongoDB\BSON\fromPHP(): Serializable returns document with null bytes in field name
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class MySerializable implements MongoDB\BSON\Serializable
{
    private $data;

    public function __construct($data)
    {
        $this->data = $data;
    }

    public function bsonSerialize()
    {
        return $this->data;
    }
}

echo "\nTesting array with one leading null byte in field name\n";
echo throws(function() {
    fromPHP(new MySerializable(["\0" => 1]));
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

echo "\nTesting array with one trailing null byte in field name\n";
echo throws(function() {
    fromPHP(new MySerializable(["a\0" => 1]));
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

echo "\nTesting array with multiple null bytes in field name\n";
echo throws(function() {
    fromPHP(new MySerializable(["\0\0\0" => 1]));
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

/* Per PHPC-884, field names with a leading null byte are ignored when encoding
 * a document from an object's property hash table, since PHP uses leading bytes
 * to denote protected and private properties. However, in this case the object
 * was returned from Serializable::bsonSerialize() and we skip the check for
 * protected and private properties. */
echo "\nTesting object with one leading null byte in field name\n";
echo throws(function() {
    fromPHP(new MySerializable((object) ["\0" => 1]));
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

echo "\nTesting object with one trailing null byte in field name\n";
echo throws(function() {
    fromPHP(new MySerializable((object) ["a\0" => 1]));
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

echo "\nTesting object with multiple null bytes in field name\n";
echo throws(function() {
    fromPHP(new MySerializable((object) ["\0\0\0" => 1]));
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing array with one leading null byte in field name
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
BSON keys cannot contain null bytes. Unexpected null byte after "".

Testing array with one trailing null byte in field name
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
BSON keys cannot contain null bytes. Unexpected null byte after "a".

Testing array with multiple null bytes in field name
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
BSON keys cannot contain null bytes. Unexpected null byte after "".

Testing object with one leading null byte in field name
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
BSON keys cannot contain null bytes. Unexpected null byte after "".

Testing object with one trailing null byte in field name
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
BSON keys cannot contain null bytes. Unexpected null byte after "a".

Testing object with multiple null bytes in field name
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
BSON keys cannot contain null bytes. Unexpected null byte after "".
===DONE===
