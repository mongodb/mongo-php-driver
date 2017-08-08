--TEST--
MongoDB\Driver\Manager::__construct(): write concern options (journal)
--FILE--
<?php

$tests = [
    ['mongodb://127.0.0.1/?journal=true', []],
    ['mongodb://127.0.0.1/?journal=false', []],
    [null, ['journal' => true]],
    [null, ['journal' => false]],
];

foreach ($tests as $test) {
    list($uri, $options) = $test;

    $manager = new MongoDB\Driver\Manager($uri, $options);
    var_dump($manager->getWriteConcern());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["j"]=>
  bool(true)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["j"]=>
  bool(false)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["j"]=>
  bool(true)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["j"]=>
  bool(false)
}
===DONE===
