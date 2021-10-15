--TEST--
ini_get() reports mongodb.mock_service_id (master and local)
--INI--
mongodb.mock_service_id=1
--FILE--
<?php

var_dump(ini_get('mongodb.mock_service_id'));
ini_set('mongodb.mock_service_id', '0');
var_dump(ini_get('mongodb.mock_service_id'));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(1) "1"
string(1) "0"
===DONE===
