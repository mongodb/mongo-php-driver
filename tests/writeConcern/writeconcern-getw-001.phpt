--TEST--
MongoDB\Driver\WriteConcern::getW()
--FILE--
<?php

$tests = array(
    MongoDB\Driver\WriteConcern::MAJORITY,
    -3,
    -2,
    -1,
    0,
    1,
    2,
    'tag',
    '2',
);

foreach ($tests as $test) {
    $wc = new MongoDB\Driver\WriteConcern($test);
    var_dump($wc->getW());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(8) "majority"
string(8) "majority"
NULL
int(-1)
int(0)
int(1)
int(2)
string(3) "tag"
string(1) "2"
===DONE===
