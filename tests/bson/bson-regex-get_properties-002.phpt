--TEST--
MongoDB\BSON\Regex get_properties handler (foreach)
--FILE--
<?php

$regex = new MongoDB\BSON\Regex('regexp', 'i');

foreach ($regex as $key => $value) {
    var_dump($key);
    var_dump($value);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(7) "pattern"
string(6) "regexp"
string(5) "flags"
string(1) "i"
===DONE===
