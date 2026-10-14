--TEST--
MongoDB\Driver\WriteConcern::getJournal()
--FILE--
<?php

$tests = array(
    true,
    false,
    1,
    0,
    null,
);

foreach ($tests as $test) {
    $wc = new MongoDB\Driver\WriteConcern(1, 0, $test);
    var_dump($wc->getJournal());
    var_dump($wc->j);
}

// Test with default value
$wc = new MongoDB\Driver\WriteConcern(1, 0);
var_dump($wc->getJournal());
var_dump($wc->j);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
bool(false)
bool(false)
bool(true)
bool(true)
bool(false)
bool(false)
NULL
NULL
NULL
NULL
===DONE===
