--TEST--
MongoDB\Driver\ReadPreference::getHedge()
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    [],
    ['enabled' => true],
    (object) ['enabled' => true],
    ['foo' => 'bar'],
];

foreach ($tests as $test) {
    $rp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['hedge' => $test]);
    var_dump($rp->getHedge());
    var_dump($rp->hedge);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: MongoDB\Driver\ReadPreference::__construct(): The "hedge" option is deprecated as of MongoDB 8.0 and will be removed in a future release in %s

Deprecated: Method MongoDB\Driver\ReadPreference::getHedge() is deprecated in %s
NULL

Deprecated: main(): Property MongoDB\Driver\ReadPreference::hedge is deprecated in %s
NULL

Deprecated: MongoDB\Driver\ReadPreference::__construct(): The "hedge" option is deprecated as of MongoDB 8.0 and will be removed in a future release in %s

Deprecated: Method MongoDB\Driver\ReadPreference::getHedge() is deprecated in %s
object(stdClass)#%d (%d) {
  ["enabled"]=>
  bool(true)
}
object(stdClass)#%d (%d) {
  ["enabled"]=>
  bool(true)
}

Deprecated: MongoDB\Driver\ReadPreference::__construct(): The "hedge" option is deprecated as of MongoDB 8.0 and will be removed in a future release in %s

Deprecated: Method MongoDB\Driver\ReadPreference::getHedge() is deprecated in %s
object(stdClass)#%d (%d) {
  ["enabled"]=>
  bool(true)
}
object(stdClass)#%d (%d) {
  ["enabled"]=>
  bool(true)
}

Deprecated: MongoDB\Driver\ReadPreference::__construct(): The "hedge" option is deprecated as of MongoDB 8.0 and will be removed in a future release in %s

Deprecated: Method MongoDB\Driver\ReadPreference::getHedge() is deprecated in %s
object(stdClass)#%d (%d) {
  ["foo"]=>
  string(3) "bar"
}
object(stdClass)#%d (%d) {
  ["foo"]=>
  string(3) "bar"
}
===DONE===
