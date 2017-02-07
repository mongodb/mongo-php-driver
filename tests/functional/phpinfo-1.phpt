--TEST--
phpinfo() reports mongodb.debug (no value)
--SKIPIF--
<?php if (defined("HHVM_VERSION_ID")) exit("skip HHVM does not do phpinfo() this way"); ?>
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
