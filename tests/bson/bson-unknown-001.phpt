--TEST--
BSON Serializing a PHP resource should throw exception
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

throws(function() {
    $a = array("stderr" => STDERR);

    $b = fromPHP($a);
}, "MongoDB\Driver\Exception\UnexpectedValueException");

throws(function() {
    $a = array("stderr" => STDERR, "stdout" => STDOUT);

    $b = fromPHP($a);
}, "MongoDB\Driver\Exception\UnexpectedValueException");


?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
===DONE===
