--TEST--
MongoDB\Driver\ServerApi debug output
--FILE--
<?php

var_dump(new MongoDB\Driver\ServerApi('1'));
var_dump(new MongoDB\Driver\ServerApi('1', true));
var_dump(new MongoDB\Driver\ServerApi('1', null, true));
var_dump(new MongoDB\Driver\ServerApi('1', false, false));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\ServerApi)#%d (%d) {
  ["version"]=>
  string(1) "1"
  ["strict"]=>
  NULL
  ["deprecationErrors"]=>
  NULL
}
object(MongoDB\Driver\ServerApi)#%d (%d) {
  ["version"]=>
  string(1) "1"
  ["strict"]=>
  bool(true)
  ["deprecationErrors"]=>
  NULL
}
object(MongoDB\Driver\ServerApi)#%d (%d) {
  ["version"]=>
  string(1) "1"
  ["strict"]=>
  NULL
  ["deprecationErrors"]=>
  bool(true)
}
object(MongoDB\Driver\ServerApi)#%d (%d) {
  ["version"]=>
  string(1) "1"
  ["strict"]=>
  bool(false)
  ["deprecationErrors"]=>
  bool(false)
}
===DONE===
