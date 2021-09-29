--TEST--
phpinfo() reports mongodb.mock_service_id (master and local)
--INI--
mongodb.mock_service_id=1
--FILE--
<?php

/* Note: the master and local values for mongodb.mock_service_id are not
 * differentiated in phpinfo() output, since its value is only tracked by
 * mongoc_global_mock_service_id (DisplayMockServiceId). */
ini_set('mongodb.mock_service_id', '0');
phpinfo();

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%a
mongodb.mock_service_id => Off => Off
%a
===DONE===
