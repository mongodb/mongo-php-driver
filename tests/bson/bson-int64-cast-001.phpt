--TEST--
MongoDB\BSON\Int64 casting
--FILE--
<?php

$tests = [
    new MongoDB\BSON\Int64(2),
    new MongoDB\BSON\Int64(0),
];

foreach($tests as $test) {
    var_dump($test);
    var_dump((int) $test);
    var_dump((float) $test);
    var_dump((string) $test);
    var_dump((bool) $test);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(1) "2"
}
int(2)
float(2)
string(1) "2"
bool(true)
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(1) "0"
}
int(0)
float(0)
string(1) "0"
bool(false)
===DONE===
