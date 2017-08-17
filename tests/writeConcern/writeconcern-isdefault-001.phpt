--TEST--
MongoDB\Driver\WriteConcern::isDefault()
--FILE--
<?php

$tests = [
    new MongoDB\Driver\WriteConcern(1),
    (new MongoDB\Driver\Manager('mongodb://127.0.0.1/?w=1'))->getWriteConcern(),
    (new MongoDB\Driver\Manager(null, ['w' => 1]))->getWriteConcern(),
    (new MongoDB\Driver\Manager)->getWriteConcern(),
];

foreach ($tests as $wc) {
    var_dump($wc->isDefault());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(false)
bool(false)
bool(false)
bool(true)
===DONE===
