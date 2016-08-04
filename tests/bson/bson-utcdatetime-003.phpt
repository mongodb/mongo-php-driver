--TEST--
BSON BSON\UTCDateTime construction from 64-bit integer
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$classname = BSON_NAMESPACE . '\UTCDateTime';
$utcdatetime = new $classname(1416445411987);

var_dump($utcdatetime);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(%SBSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  int(1416445411987)
}
===DONE===
