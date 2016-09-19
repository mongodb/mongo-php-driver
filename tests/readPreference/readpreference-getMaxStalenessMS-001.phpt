--TEST--
MongoDB\Driver\ReadPreference::getMaxStalenessMS()
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    0,
    1000,
    2147483647,
];

foreach ($tests as $test) {
    $rp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, null, ['maxStalenessMS' => $test]);
    var_dump($rp->getMaxStalenessMS());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
int(0)
int(1000)
int(2147483647)
===DONE===
