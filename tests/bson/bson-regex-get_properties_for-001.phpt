--TEST--
MongoDB\BSON\Regex get_properties_for handler (var_dump, print_r, array cast, var_export)
--FILE--
<?php

$regex = new MongoDB\BSON\Regex('regexp', 'i');

var_dump($regex);

print_r($regex);

var_dump((array) $regex);

var_export($regex);
echo "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Regex)#%d (2) {
  ["pattern"]=>
  string(6) "regexp"
  ["flags"]=>
  string(1) "i"
}
MongoDB\BSON\Regex Object
(
    [pattern] => regexp
    [flags] => i
)
array(2) {
  ["pattern"]=>
  string(6) "regexp"
  ["flags"]=>
  string(1) "i"
}
%r\\?%rMongoDB\BSON\Regex::__set_state(array(
   'pattern' => 'regexp',
   'flags' => 'i',
))
===DONE===
