--TEST--
MongoDB\BSON\Binary get_properties handler (foreach)
--FILE--
<?php

$binary = new MongoDB\BSON\Binary('foobar', MongoDB\BSON\Binary::TYPE_GENERIC);

foreach ($binary as $key => $value) {
    var_dump($key);
    var_dump($value);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(4) "data"
string(6) "foobar"
string(4) "type"
int(0)
===DONE===
