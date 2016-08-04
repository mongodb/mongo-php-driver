--TEST--
MongoDB\BSON\Regex::__set_state() requires "pattern" and "flags" string fields
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    MongoDB\BSON\Regex::__set_state(['pattern' => 'regexp']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    MongoDB\BSON\Regex::__set_state(['flags' => 'i']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    MongoDB\BSON\Regex::__set_state(['pattern' => 0, 'flags' => 0]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Regex initialization requires "pattern" and "flags" string fields
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Regex initialization requires "pattern" and "flags" string fields
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Regex initialization requires "pattern" and "flags" string fields
===DONE===
