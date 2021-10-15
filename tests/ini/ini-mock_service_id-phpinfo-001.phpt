--TEST--
phpinfo() reports mongodb.mock_service_id (default)
--FILE--
<?php

phpinfo();

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%a
mongodb.mock_service_id => Off => Off
%a
===DONE===
