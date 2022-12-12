--TEST--
MongoDB\BSON\ArrayList::__set_state()
--FILE--
<?php

$array = MongoDB\BSON\ArrayList::fromPHP([
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
%r\\?%rMongoDB\BSON\ArrayList::__set_state(array(
%w'data' => 'MwAAABAwAAAAAAAQMQABAAAAAjIABAAAAGZvbwADMwASAAAAAmZvbwAEAAAAYmFyAAAA',
))
object(MongoDB\BSON\ArrayList)#%d (%d) {
  ["data"]=>
  string(68) "MwAAABAwAAAAAAAQMQABAAAAAjIABAAAAGZvbwADMwASAAAAAmZvbwAEAAAAYmFyAAAA"
}
bool(true)
===DONE===
