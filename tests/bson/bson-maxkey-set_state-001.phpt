--TEST--
MongoDB\BSON\MaxKey::__set_state()
--FILE--
<?php

var_export(MongoDB\BSON\MaxKey::__set_state([]));
echo "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%r\\?%rMongoDB\BSON\MaxKey::__set_state(array(
))
===DONE===
