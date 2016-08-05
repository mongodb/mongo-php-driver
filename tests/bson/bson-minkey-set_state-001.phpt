--TEST--
MongoDB\BSON\MinKey::__set_state()
--FILE--
<?php

var_export(MongoDB\BSON\MinKey::__set_state([]));
echo "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
MongoDB\BSON\MinKey::__set_state(array(
))
===DONE===
