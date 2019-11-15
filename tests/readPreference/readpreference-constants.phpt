--TEST--
MongoDB\Driver\ReadPreference constants
--FILE--
<?php

var_dump(MongoDB\Driver\ReadPreference::RP_PRIMARY);
var_dump(MongoDB\Driver\ReadPreference::RP_PRIMARY_PREFERRED);
var_dump(MongoDB\Driver\ReadPreference::RP_SECONDARY);
var_dump(MongoDB\Driver\ReadPreference::RP_SECONDARY_PREFERRED);
var_dump(MongoDB\Driver\ReadPreference::RP_NEAREST);
var_dump(MongoDB\Driver\ReadPreference::NO_MAX_STALENESS);
var_dump(MongoDB\Driver\ReadPreference::SMALLEST_MAX_STALENESS_SECONDS);

var_dump(MongoDB\Driver\ReadPreference::PRIMARY);
var_dump(MongoDB\Driver\ReadPreference::PRIMARY_PREFERRED);
var_dump(MongoDB\Driver\ReadPreference::SECONDARY);
var_dump(MongoDB\Driver\ReadPreference::SECONDARY_PREFERRED);
var_dump(MongoDB\Driver\ReadPreference::NEAREST);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
int(1)
int(5)
int(2)
int(6)
int(10)
int(-1)
int(90)
string(7) "primary"
string(16) "primaryPreferred"
string(9) "secondary"
string(18) "secondaryPreferred"
string(7) "nearest"
===DONE===
