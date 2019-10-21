--TEST--
MongoDB\Driver\ReadConcern serialization
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
    var_dump($test instanceof Serializable);
    echo $s = serialize($test), "\n";
    var_dump(unserialize($s));
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\ReadConcern)#%d (%d) {
}
bool(true)
C:26:"MongoDB\Driver\ReadConcern":0:{}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
}

object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(12) "linearizable"
}
bool(true)
C:26:"MongoDB\Driver\ReadConcern":38:{a:1:{s:5:"level";s:12:"linearizable";}}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(12) "linearizable"
}

object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(5) "local"
}
bool(true)
C:26:"MongoDB\Driver\ReadConcern":30:{a:1:{s:5:"level";s:5:"local";}}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(5) "local"
}

object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(8) "majority"
}
bool(true)
C:26:"MongoDB\Driver\ReadConcern":33:{a:1:{s:5:"level";s:8:"majority";}}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(8) "majority"
}

object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(9) "available"
}
bool(true)
C:26:"MongoDB\Driver\ReadConcern":34:{a:1:{s:5:"level";s:9:"available";}}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(9) "available"
}

===DONE===
