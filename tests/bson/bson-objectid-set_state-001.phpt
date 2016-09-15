--TEST--
MongoDB\BSON\ObjectID::__set_state()
--FILE--
<?php

var_export(MongoDB\BSON\ObjectID::__set_state([
  'oid' => '576c25db6118fd406e6e6471',
]));
echo "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
MongoDB\BSON\ObjectID::__set_state(array(
%w'oid' => '576c25db6118fd406e6e6471',
))
===DONE===
