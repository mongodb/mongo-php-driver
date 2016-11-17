--TEST--
MongoDB\Driver\ReadPreference::getMaxStalenessSeconds()
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    0.5,
    1.000,
    2147483.647,
];

foreach ($tests as $test) {
    $rp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, null, ['maxStalenessSeconds' => $test]);
    var_dump($rp->getMaxStalenessSeconds());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
float(0.5)
float(1)
float(2147483.647)
===DONE===
