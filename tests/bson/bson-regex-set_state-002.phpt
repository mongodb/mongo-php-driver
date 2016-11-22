--TEST--
MongoDB\BSON\Regex::__set_state() will alphabetize flags
--FILE--
<?php

var_export(MongoDB\BSON\Regex::__set_state([
    'pattern' => 'regexp',
    'flags' => 'xusmli',
]));
echo "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
MongoDB\BSON\Regex::__set_state(array(
%w'pattern' => 'regexp',
%w'flags' => 'ilmsux',
))
===DONE===
