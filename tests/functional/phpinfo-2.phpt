--TEST--
phpinfo() reports mongodb.debug (default and overridden)
--INI--
mongodb.debug=stderr
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
