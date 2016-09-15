--TEST--
MongoDB\Driver\ReadPreference::getMode()
--FILE--
<?php

$tests = [
    MongoDB\Driver\ReadPreference::RP_PRIMARY,
    MongoDB\Driver\ReadPreference::RP_PRIMARY_PREFERRED,
    MongoDB\Driver\ReadPreference::RP_SECONDARY,
    MongoDB\Driver\ReadPreference::RP_SECONDARY_PREFERRED,
    MongoDB\Driver\ReadPreference::RP_NEAREST,
];

foreach ($tests as $test) {
    $rp = new MongoDB\Driver\ReadPreference($test);
    var_dump($rp->getMode());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
int(1)
int(5)
int(2)
int(6)
int(10)
===DONE===
