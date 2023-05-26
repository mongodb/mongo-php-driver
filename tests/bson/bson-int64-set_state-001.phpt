--TEST--
MongoDB\BSON\Int64::__set_state()
--FILE--
<?php

$int64 = new MongoDB\BSON\Int64(2);

$s = var_export($int64, true);
echo $s, "\n";

var_dump(eval('return ' . $s . ';'));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%r\\?%rMongoDB\BSON\Int64::__set_state(array(
%w'integer' => '2',
))
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(1) "2"
}
===DONE===
