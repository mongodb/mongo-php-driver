--TEST--
MongoDB\Driver\ReadConcern::isDefault()
--FILE--
<?php

$tests = [
    new MongoDB\Driver\ReadConcern(),
    new MongoDB\Driver\ReadConcern(null),
    new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LOCAL),
    new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::MAJORITY),
    new MongoDB\Driver\ReadConcern('not-yet-supported'),
    (new MongoDB\Driver\Manager('mongodb://127.0.0.1/?readconcernlevel=local'))->getReadConcern(),
    (new MongoDB\Driver\Manager('mongodb://127.0.0.1/?readconcernlevel='))->getReadConcern(),
    (new MongoDB\Driver\Manager(null, ['readconcernlevel' => 'local']))->getReadConcern(),
    (new MongoDB\Driver\Manager(null, ['readconcernlevel' => '']))->getReadConcern(),
    // Cannot test ['readconcernlevel' => null] since a string type is expected (PHPC-887)
    (new MongoDB\Driver\Manager)->getReadConcern(),
];

foreach ($tests as $rc) {
    var_dump($rc->isDefault());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(true)
===DONE===
