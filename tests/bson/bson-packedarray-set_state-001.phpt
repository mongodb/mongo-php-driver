--TEST--
MongoDB\BSON\PackedArray::__set_state()
--FILE--
<?php

$array = MongoDB\BSON\PackedArray::fromPHP([
    0,
    1,
    'foo',
    (object) ['foo' => 'bar']
]);
$s = var_export($array, true);
echo $s, "\n";

var_dump($a = eval('return ' . $s . ';'));
var_dump($a == $array);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%r\\?%rMongoDB\BSON\PackedArray::__set_state(array(
%w'data' => 'MwAAABAwAAAAAAAQMQABAAAAAjIABAAAAGZvbwADMwASAAAAAmZvbwAEAAAAYmFyAAAA',
))
object(MongoDB\BSON\PackedArray)#%d (%d) {
  ["data"]=>
  string(68) "MwAAABAwAAAAAAAQMQABAAAAAjIABAAAAGZvbwADMwASAAAAAmZvbwAEAAAAYmFyAAAA"
  ["value"]=>
  array(4) {
    [0]=>
    int(0)
    [1]=>
    int(1)
    [2]=>
    string(3) "foo"
    [3]=>
    object(MongoDB\BSON\Document)#%d (%d) {
      ["data"]=>
      string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
      ["value"]=>
      object(stdClass)#%d (%d) {
        ["foo"]=>
        string(3) "bar"
      }
    }
  }
}
bool(true)
===DONE===
