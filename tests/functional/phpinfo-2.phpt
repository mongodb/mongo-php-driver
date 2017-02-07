--TEST--
phpinfo() reports mongodb.debug (default and overridden)
--INI--
mongodb.debug=stderr
--SKIPIF--
<?php if (defined("HHVM_VERSION_ID")) exit("skip HHVM does not do phpinfo() this way"); ?>
--FILE--
<?php

ini_set("mongodb.debug", "stdout");
phpinfo();

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%a
mongodb.debug => stdout => stderr
%a
===DONE===
