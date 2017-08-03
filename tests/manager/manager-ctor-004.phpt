--TEST--
MongoDB\Driver\Manager::__construct(): Deprecated boolean options in URI string
--FILE--
<?php

$tests = [
    // Deprecated URI string values for true
    '1',
    'yes',
    'y',
    't',
    // Deprecated URI string values for false
    '0',
    '-1',
    'no',
    'n',
    'f',
];

foreach ($tests as $test) {
    $manager = new MongoDB\Driver\Manager('mongodb://127.0.0.1/?journal=' . $test);
    var_dump($manager->getWriteConcern()->getJournal());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
===DONE===
