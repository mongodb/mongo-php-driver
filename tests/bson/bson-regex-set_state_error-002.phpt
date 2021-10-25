--TEST--
MongoDB\BSON\Regex::__set_state() does not allow pattern or flags to contain null bytes
--DESCRIPTION--
BSON Corpus spec prose test #1
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    MongoDB\BSON\Regex::__set_state(['pattern' => "regexp\0", 'flags' => 'i']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    MongoDB\BSON\Regex::__set_state(['pattern' => 'regexp', 'flags' => "i\0"]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Pattern cannot contain null bytes
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Flags cannot contain null bytes
===DONE===
