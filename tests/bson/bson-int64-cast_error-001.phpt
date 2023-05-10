--TEST--
MongoDB\BSON\Int64 casting errors
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$int64 = new MongoDB\BSON\Int64(2);

echo raises(function () use ($int64) {
    var_dump((float) $int64);
}, E_NOTICE | E_WARNING), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got %r(E_NOTICE|E_WARNING)%r
Object of class MongoDB\BSON\Int64 could not be converted to float
===DONE===
