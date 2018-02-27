--TEST--
MongoDB\Driver\Query: Invalid types
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

throws(function() {
    $query = new MongoDB\Driver\Query(array("company" => "Smith, Carter and Buckridge"), array(
        "projection" => array("_id" => 0, "username" => 1),
        "sort" => array("phoneNumber" => 1),
        "modifiers" => "string",
    ));
}, "MongoDB\Driver\Exception\InvalidArgumentException");

throws(function() {
    $query = new MongoDB\Driver\Query(array("company" => "Smith, Carter and Buckridge"), array(
        "projection" => array("_id" => 0, "username" => 1),
        "sort" => array("phoneNumber" => 1),
        "projection" => "string",
    ));
}, "MongoDB\Driver\Exception\InvalidArgumentException");

throws(function() {
    $query = new MongoDB\Driver\Query(array("company" => "Smith, Carter and Buckridge"), array(
        "projection" => array("_id" => 0, "username" => 1),
        "sort" => "string"
    ));
}, "MongoDB\Driver\Exception\InvalidArgumentException");

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
===DONE===
