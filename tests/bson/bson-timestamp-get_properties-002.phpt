--TEST--
MongoDB\BSON\Timestamp get_properties handler (foreach)
--FILE--
<?php

$timestamp = new MongoDB\BSON\Timestamp(1234, 5678);

foreach ($timestamp as $key => $value) {
    var_dump($key);
    var_dump($value);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(9) "increment"
string(4) "1234"
string(9) "timestamp"
string(4) "5678"
===DONE===
