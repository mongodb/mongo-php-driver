--TEST--
MongoDB\Driver\ServerApi::__set_state()
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    ['version' => '1'],
    ['version' => '1', 'strict' => true],
    ['version' => '1', 'deprecationErrors' => true],
    ['version' => '1', 'strict' => false, 'deprecationErrors' => false],
    ['version' => '1', 'strict' => null, 'deprecationErrors' => null],
];

foreach ($tests as $fields) {
    var_export(MongoDB\Driver\ServerApi::__set_state($fields));
    echo "\n\n";
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

%r\\?%rMongoDB\Driver\ServerApi::__set_state(array(
   'version' => '1',
   'strict' => NULL,
   'deprecationErrors' => NULL,
))

===DONE===
