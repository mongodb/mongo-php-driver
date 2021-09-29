--TEST--
ini_get() reports mongodb.debug (default)
--FILE--
<?php

var_dump(ini_get('mongodb.debug'));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(0) ""
===DONE===
