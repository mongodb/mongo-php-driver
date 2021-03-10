--TEST--
MongoDB\Driver\ServerApi::__set_state() requires correct data types and values
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    MongoDB\Driver\ServerApi::__set_state([]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    MongoDB\Driver\ServerApi::__set_state(['version' => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    MongoDB\Driver\ServerApi::__set_state(['version' => '2']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";


echo throws(function() {
    MongoDB\Driver\ServerApi::__set_state(['version' => '1', 'strict' => 'true']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";


echo throws(function() {
    MongoDB\Driver\ServerApi::__set_state(['version' => '1', 'deprecationErrors' => 'true']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ServerApi initialization requires "version" field to be string
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ServerApi initialization requires "version" field to be string
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Server API version "2" is not supported in this driver version
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ServerApi initialization requires "strict" field to be bool or null
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ServerApi initialization requires "deprecationErrors" field to be bool or null
===DONE===
