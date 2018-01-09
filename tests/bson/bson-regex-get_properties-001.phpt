--TEST--
MongoDB\BSON\Regex get_properties handler (get_object_vars)
--FILE--
<?php

$regex = new MongoDB\BSON\Regex('regexp', 'i');

var_dump(get_object_vars($regex));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(2) {
  ["pattern"]=>
  string(6) "regexp"
  ["flags"]=>
  string(1) "i"
}
===DONE===
