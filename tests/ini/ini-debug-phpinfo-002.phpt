--TEST--
phpinfo() reports mongodb.debug (master and local)
--INI--
mongodb.debug=stderr
--FILE--
<?php

ini_set('mongodb.debug', 'stdout');
phpinfo();

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%a
mongodb.debug => stdout => stderr
%a
===DONE===%A
