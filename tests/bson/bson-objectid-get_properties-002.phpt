--TEST--
MongoDB\BSON\ObjectId get_properties handler (foreach)
--FILE--
<?php

$oid = new MongoDB\BSON\ObjectId('53e2a1c40640fd72175d4603');

foreach ($oid as $key => $value) {
    var_dump($key);
    var_dump($value);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(3) "oid"
string(24) "53e2a1c40640fd72175d4603"
===DONE===
