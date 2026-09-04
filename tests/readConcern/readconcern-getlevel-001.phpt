--TEST--
MongoDB\Driver\ReadConcern::getLevel()
--FILE--
<?php

$tests = [
    null,
    MongoDB\Driver\ReadConcern::LOCAL,
    'not-yet-supported',
];

foreach ($tests as $test) {
    $rc = new MongoDB\Driver\ReadConcern($test);
    var_dump($rc->getLevel());
    var_dump($rc->level);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
NULL
NULL
string(5) "local"
string(5) "local"
string(17) "not-yet-supported"
string(17) "not-yet-supported"
===DONE===
