--TEST--
ExecutionTimeoutException: exceeding $maxTimeMS (queries)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
<?php skip_if_test_commands_disabled(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$query = new MongoDB\Driver\Query(array("company" => "Smith, Carter and Buckridge"), array(
    'projection' => array('_id' => 0, 'username' => 1),
    'sort' => array('phoneNumber' => 1),
    'modifiers' => array(
        '$maxTimeMS' => 1,
    ),
));

failMaxTimeMS($manager);
throws(function() use ($manager, $query) {
    $result = $manager->executeQuery(NS, $query);
}, "MongoDB\Driver\Exception\ExecutionTimeoutException");

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\ExecutionTimeoutException
===DONE===
