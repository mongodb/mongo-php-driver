--TEST--
MongoDB\Driver\ReadPreference::getModeString()
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
    var_dump($rp->getModeString());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(7) "primary"
string(16) "primaryPreferred"
string(9) "secondary"
string(18) "secondaryPreferred"
string(7) "nearest"
===DONE===
