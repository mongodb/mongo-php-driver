--TEST--
ini_get() reports mongodb.mock_service_id (default)
--FILE--
<?php

var_dump(ini_get('mongodb.mock_service_id'));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(1) "0"
===DONE===
