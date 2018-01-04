--TEST--
MongoDB\Driver\ReadConcern debug output
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    new MongoDB\Driver\ReadConcern(),
    new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LINEARIZABLE),
    new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LOCAL),
    new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::MAJORITY),
    new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::AVAILABLE),
];

foreach ($tests as $test) {
    var_dump($test);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\ReadConcern)#%d (%d) {
}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(12) "linearizable"
}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(5) "local"
}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(8) "majority"
}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(9) "available"
}
===DONE===
