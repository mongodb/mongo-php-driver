--TEST--
MongoDB\Driver\ServerApi::__construct()
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$version = new MongoDB\Driver\ServerApi('1');

echo throws(function() {
    new MongoDB\Driver\ServerApi('no way');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Server API version "no way" is not supported in this driver version
===DONE===
