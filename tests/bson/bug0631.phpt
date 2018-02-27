--TEST--
PHPC-631: UTCDateTime::toDateTime() may return object that cannot be serialized
--INI--
date.timezone=UTC
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$utcdatetime = new MongoDB\BSON\UTCDateTime('1466540755123');
$datetime = $utcdatetime->toDateTime();
$s = serialize($datetime);

var_dump($datetime);

echo "\n", $s, "\n\n";

var_dump(unserialize($s));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(DateTime)#%d (%d) {
  ["date"]=>
  string(26) "2016-06-21 20:25:55.123000"
  ["timezone_type"]=>
  int(1)
  ["timezone"]=>
  string(6) "+00:00"
}

O:8:"DateTime":3:{s:4:"date";s:26:"2016-06-21 20:25:55.123000";s:13:"timezone_type";i:1;s:8:"timezone";s:6:"+00:00";}

object(DateTime)#%d (%d) {
  ["date"]=>
  string(26) "2016-06-21 20:25:55.123000"
  ["timezone_type"]=>
  int(1)
  ["timezone"]=>
  string(6) "+00:00"
}
===DONE===
