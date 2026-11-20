--TEST--
MongoDB\Driver\ReadPreference::getModeString()
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
    var_dump($rp->getModeString());
    var_dump($rp->mode);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(7) "primary"
string(7) "primary"
string(16) "primaryPreferred"
string(16) "primaryPreferred"
string(9) "secondary"
string(9) "secondary"
string(18) "secondaryPreferred"
string(18) "secondaryPreferred"
string(7) "nearest"
string(7) "nearest"
===DONE===
