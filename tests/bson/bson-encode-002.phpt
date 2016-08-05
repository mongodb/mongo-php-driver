--TEST--
BSON encoding: Encoding objects into BSON representation
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class AssociativeArray implements MongoDB\BSON\Serializable, MongoDB\BSON\Unserializable
{
    public function bsonSerialize()
    {
        return array("random" => "class", "data");
    }

    public function bsonUnserialize(array $data)
    {
        echo __METHOD__, "() was called with data:\n";
        var_dump($data);
    }
}

class NumericArray implements MongoDB\BSON\Serializable, MongoDB\BSON\Unserializable
{
    public function bsonSerialize()
    {
        return array(1, 2, 3);
    }

    public function bsonUnserialize(array $data)
    {
        echo __METHOD__, "() was called with data:\n";
        var_dump($data);
    }
}

echo "Testing top-level AssociativeArray:\n";
$bson = fromPHP(new AssociativeArray);
echo toJSON($bson), "\n";
echo "Encoded BSON:\n";
hex_dump($bson);
$value = toPHP($bson, array("root" => 'AssociativeArray'));
echo "Decoded BSON:\n";
var_dump($value);

echo "\nTesting embedded AssociativeArray:\n";
$bson = fromPHP(array('embed' => new AssociativeArray));
echo toJSON($bson), "\n";
echo "Encoded BSON:\n";
hex_dump($bson);
$value = toPHP($bson, array("document" => 'AssociativeArray'));
echo "Decoded BSON:\n";
var_dump($value);

echo "\nTesting top-level NumericArray:\n";
$bson = fromPHP(new NumericArray);
echo toJSON($bson), "\n";
echo "Encoded BSON:\n";
hex_dump($bson);
$value = toPHP($bson, array("root" => 'NumericArray'));
echo "Decoded BSON:\n";
var_dump($value);

echo "\nTesting embedded NumericArray:\n";
$bson = fromPHP(array('embed' => new NumericArray));
echo toJSON($bson), "\n";
echo "Encoded BSON:\n";
hex_dump($bson);
$value = toPHP($bson, array("array" => 'NumericArray'));
echo "Decoded BSON:\n";
var_dump($value);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing top-level AssociativeArray:
{ "random" : "class", "0" : "data" }
Encoded BSON:
     0 : 23 00 00 00 02 72 61 6e 64 6f 6d 00 06 00 00 00  [#....random.....]
    10 : 63 6c 61 73 73 00 02 30 00 05 00 00 00 64 61 74  [class..0.....dat]
    20 : 61 00 00                                         [a..]
AssociativeArray::bsonUnserialize() was called with data:
array(2) {
  ["random"]=>
  string(5) "class"
  [0]=>
  string(4) "data"
}
Decoded BSON:
object(AssociativeArray)#%d (0) {
}

Testing embedded AssociativeArray:
{ "embed" : { "random" : "class", "0" : "data" } }
Encoded BSON:
     0 : 2f 00 00 00 03 65 6d 62 65 64 00 23 00 00 00 02  [/....embed.#....]
    10 : 72 61 6e 64 6f 6d 00 06 00 00 00 63 6c 61 73 73  [random.....class]
    20 : 00 02 30 00 05 00 00 00 64 61 74 61 00 00 00     [..0.....data...]
AssociativeArray::bsonUnserialize() was called with data:
array(2) {
  ["random"]=>
  string(5) "class"
  [0]=>
  string(4) "data"
}
Decoded BSON:
object(stdClass)#%d (1) {
  ["embed"]=>
  object(AssociativeArray)#%d (0) {
  }
}

Testing top-level NumericArray:
{ "0" : 1, "1" : 2, "2" : 3 }
Encoded BSON:
     0 : 1a 00 00 00 10 30 00 01 00 00 00 10 31 00 02 00  [.....0......1...]
    10 : 00 00 10 32 00 03 00 00 00 00                    [...2......]
NumericArray::bsonUnserialize() was called with data:
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
Decoded BSON:
object(NumericArray)#%d (0) {
}

Testing embedded NumericArray:
{ "embed" : [ 1, 2, 3 ] }
Encoded BSON:
     0 : 26 00 00 00 04 65 6d 62 65 64 00 1a 00 00 00 10  [&....embed......]
    10 : 30 00 01 00 00 00 10 31 00 02 00 00 00 10 32 00  [0......1......2.]
    20 : 03 00 00 00 00 00                                [......]
NumericArray::bsonUnserialize() was called with data:
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
Decoded BSON:
object(stdClass)#%d (1) {
  ["embed"]=>
  object(NumericArray)#%d (0) {
  }
}
===DONE===
