--TEST--
MongoDB\BSON\Document::__set_state()
--FILE--
<?php

$s = var_export(MongoDB\BSON\Document::fromPHP(['_id' => 1, 'array' => [0, 1, 'foo'], 'document' => ['foo' => 'bar']]), true);
echo $s, "\n";

var_dump(eval('return ' . $s . ';'));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%r\\?%rMongoDB\BSON\Document::__set_state(array(
%w'data' => 'TwAAABBfaWQAAQAAAARhcnJheQAeAAAAEDAAAAAAABAxAAEAAAACMgAEAAAAZm9vAAADZG9jdW1lbnQAEgAAAAJmb28ABAAAAGJhcgAAAA==',
%w'length' => 79,
))
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(108) "TwAAABBfaWQAAQAAAARhcnJheQAeAAAAEDAAAAAAABAxAAEAAAACMgAEAAAAZm9vAAADZG9jdW1lbnQAEgAAAAJmb28ABAAAAGJhcgAAAA=="
  ["length"]=>
  int(79)
}
===DONE===
