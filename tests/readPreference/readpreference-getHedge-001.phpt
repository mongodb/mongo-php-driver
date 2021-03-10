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
    $rp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, null, ['hedge' => $test]);
    var_dump($rp->getHedge());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
NULL
object(stdClass)#%d (%d) {
  ["enabled"]=>
  bool(true)
}
object(stdClass)#%d (%d) {
  ["enabled"]=>
  bool(true)
}
object(stdClass)#%d (%d) {
  ["foo"]=>
  string(3) "bar"
}
===DONE===
