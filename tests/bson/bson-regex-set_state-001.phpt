--TEST--
MongoDB\BSON\Regex::__set_state()
--FILE--
<?php

var_export(MongoDB\BSON\Regex::__set_state([
    'pattern' => 'regexp',
    'flags' => 'i',
]));
echo "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%r\\?%rMongoDB\BSON\Regex::__set_state(array(
%w'pattern' => 'regexp',
%w'flags' => 'i',
))
===DONE===
