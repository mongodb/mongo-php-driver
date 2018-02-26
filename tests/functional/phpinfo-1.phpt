--TEST--
phpinfo() reports mongodb.debug (no value)
--FILE--
<?php

phpinfo();

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%a
mongodb.debug => no value => no value
%a
===DONE===
