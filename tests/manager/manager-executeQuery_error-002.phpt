--TEST--
MongoDB\Driver\Manager::executeQuery() with invalid options
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$query = new MongoDB\Driver\Query(['x' => 3], ['projection' => ['y' => 1]]);

echo throws(function() use ($manager, $query) {
    $manager->executeQuery(NS, $query, ['readPreference' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($manager, $query) {
    $manager->executeQuery(NS, $query, ['readPreference' => new stdClass]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($manager, $query) {
    $manager->executeQuery(NS, $query, ['session' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($manager, $query) {
    $manager->executeQuery(NS, $query, ['session' => new stdClass]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readPreference" option to be MongoDB\Driver\ReadPreference, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readPreference" option to be MongoDB\Driver\ReadPreference, stdClass given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "session" option to be MongoDB\Driver\Session, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "session" option to be MongoDB\Driver\Session, stdClass given
===DONE===
