--TEST--
MongoDB\BSON\MaxKeyInterface is implemented by MongoDB\BSON\MaxKey
--FILE--
<?php

$maxkey = new MongoDB\BSON\MaxKey;
var_dump($maxkey instanceof MongoDB\BSON\MaxKeyInterface);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===
