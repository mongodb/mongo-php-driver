--TEST--
MongoDB\BSON\ArrayList::__set_state()
--FILE--
<?php

$s = var_export(MongoDB\BSON\ArrayList::fromPHP([0, 1, 'foo', (object) ['foo' => 'bar']]), true);
echo $s, "\n";

var_dump(eval('return ' . $s . ';'));

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
===DONE===
