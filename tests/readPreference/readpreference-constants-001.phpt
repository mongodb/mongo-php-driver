--TEST--
MongoDB\Driver\ReadPreference constants
--FILE--
<?php

var_dump(MongoDB\Driver\ReadPreference::PRIMARY);
var_dump(MongoDB\Driver\ReadPreference::PRIMARY_PREFERRED);
var_dump(MongoDB\Driver\ReadPreference::SECONDARY);
var_dump(MongoDB\Driver\ReadPreference::SECONDARY_PREFERRED);
var_dump(MongoDB\Driver\ReadPreference::NEAREST);

var_dump(MongoDB\Driver\ReadPreference::NO_MAX_STALENESS);
var_dump(MongoDB\Driver\ReadPreference::SMALLEST_MAX_STALENESS_SECONDS);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
string(7) "primary"
string(16) "primaryPreferred"
string(9) "secondary"
string(18) "secondaryPreferred"
string(7) "nearest"
int(-1)
int(90)
===DONE===
