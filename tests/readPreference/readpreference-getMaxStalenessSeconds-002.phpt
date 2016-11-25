--TEST--
MongoDB\Driver\ReadPreference::getMaxStalenessSeconds() with default value
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    [],
    ['maxStalenessSeconds' => -1],
    ['maxStalenessSeconds' => -1.0],
];

foreach ($tests as $test) {
    $rp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, null, $test);
    var_dump($rp->getMaxStalenessSeconds());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
NULL
NULL
NULL
===DONE===
