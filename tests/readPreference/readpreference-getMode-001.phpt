--TEST--
MongoDB\Driver\ReadPreference::getMode()
--FILE--
<?php

$tests = [
    MongoDB\Driver\ReadPreference::PRIMARY,
    MongoDB\Driver\ReadPreference::PRIMARY_PREFERRED,
    MongoDB\Driver\ReadPreference::SECONDARY,
    MongoDB\Driver\ReadPreference::SECONDARY_PREFERRED,
    MongoDB\Driver\ReadPreference::NEAREST,
];

foreach ($tests as $test) {
    $rp = new MongoDB\Driver\ReadPreference($test);
    var_dump($rp->getMode());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: %s MongoDB\Driver\ReadPreference::getMode() is deprecated in %s
int(1)

Deprecated: %s MongoDB\Driver\ReadPreference::getMode() is deprecated in %s
int(5)

Deprecated: %s MongoDB\Driver\ReadPreference::getMode() is deprecated in %s
int(2)

Deprecated: %s MongoDB\Driver\ReadPreference::getMode() is deprecated in %s
int(6)

Deprecated: %s MongoDB\Driver\ReadPreference::getMode() is deprecated in %s
int(10)
===DONE===
