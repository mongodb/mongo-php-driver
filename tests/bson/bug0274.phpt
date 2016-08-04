--TEST--
Test for PHPC-274: zval_to_bson() should process BSON\Serializable instances
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class AssociativeArray implements MongoDB\BSON\Serializable
{
    public function bsonSerialize()
    {
        return array("random" => "class", "data");
    }
}

class NumericArray implements MongoDB\BSON\Serializable
{
    public function bsonSerialize()
    {
        return array(1, 2, 3);
    }
}

echo "Testing top-level AssociativeArray:\n";
$bson = fromPHP(new AssociativeArray);
echo toJSON($bson), "\n";
echo "Encoded BSON:\n";
hex_dump($bson);

echo "\nTesting top-level NumericArray:\n";
$bson = fromPHP(new NumericArray);
echo toJSON($bson), "\n";
echo "Encoded BSON:\n";
hex_dump($bson);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing top-level AssociativeArray:
{ "random" : "class", "0" : "data" }
Encoded BSON:
     0 : 23 00 00 00 02 72 61 6e 64 6f 6d 00 06 00 00 00  [#....random.....]
    10 : 63 6c 61 73 73 00 02 30 00 05 00 00 00 64 61 74  [class..0.....dat]
    20 : 61 00 00                                         [a..]

Testing top-level NumericArray:
{ "0" : 1, "1" : 2, "2" : 3 }
Encoded BSON:
     0 : 1a 00 00 00 10 30 00 01 00 00 00 10 31 00 02 00  [.....0......1...]
    10 : 00 00 10 32 00 03 00 00 00 00                    [...2......]
===DONE===
