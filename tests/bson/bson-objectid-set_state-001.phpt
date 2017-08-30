--TEST--
MongoDB\BSON\ObjectId::__set_state()
--FILE--
<?php

var_export(MongoDB\BSON\ObjectId::__set_state([
  'oid' => '576c25db6118fd406e6e6471',
]));
echo "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
MongoDB\BSON\ObjectId::__set_state(array(
%w'oid' => '576c25db6118fd406e6e6471',
))
===DONE===
