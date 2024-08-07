--TEST--
MongoDB\Driver\ReadPreference::getMaxStalenessSeconds()
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    MongoDB\Driver\ReadPreference::NO_MAX_STALENESS,
    MongoDB\Driver\ReadPreference::SMALLEST_MAX_STALENESS_SECONDS,
    90,
    1000,
    2147483647,
];

foreach ($tests as $test) {
    $rp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['maxStalenessSeconds' => $test]);
    var_dump($rp->getMaxStalenessSeconds());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
int(-1)
int(90)
int(90)
int(1000)
int(2147483647)
===DONE===
