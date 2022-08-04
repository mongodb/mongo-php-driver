--TEST--
MongoDB\Driver\ServerApi: var_export()
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    new MongoDB\Driver\ServerApi('1'),
    new MongoDB\Driver\ServerApi('1', true),
    new MongoDB\Driver\ServerApi('1', null, true),
    new MongoDB\Driver\ServerApi('1', false, false),
];

foreach ($tests as $test) {
    echo var_export($test, true), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%r\\?%rMongoDB\Driver\ServerApi::__set_state(array(
   'version' => '1',
   'strict' => NULL,
   'deprecationErrors' => NULL,
))
%r\\?%rMongoDB\Driver\ServerApi::__set_state(array(
   'version' => '1',
   'strict' => true,
   'deprecationErrors' => NULL,
))
%r\\?%rMongoDB\Driver\ServerApi::__set_state(array(
   'version' => '1',
   'strict' => NULL,
   'deprecationErrors' => true,
))
%r\\?%rMongoDB\Driver\ServerApi::__set_state(array(
   'version' => '1',
   'strict' => false,
   'deprecationErrors' => false,
))
===DONE===
