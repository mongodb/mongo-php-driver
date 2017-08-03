--TEST--
MongoDB\Driver\Manager::__construct(): read preference options (slaveok)
--FILE--
<?php

$tests = [
    ['mongodb://127.0.0.1/?slaveok=true', []],
    ['mongodb://127.0.0.1/?slaveok=false', []],
    // True array option will override any read preference in the URI string
    ['mongodb://127.0.0.1/?slaveok=false', ['slaveok' => true]],
    ['mongodb://127.0.0.1/?readPreference=nearest', ['slaveok' => true]],
    // False array option is ignored
    ['mongodb://127.0.0.1/?slaveok=true', ['slaveok' => false]],
    ['mongodb://127.0.0.1/?readPreference=nearest', ['slaveok' => false]],
    // readPreference option takes priority
    ['mongodb://127.0.0.1/?slaveok=true&readPreference=nearest', []],
    ['mongodb://127.0.0.1/?slaveok=false&readPreference=nearest', []],
    ['mongodb://127.0.0.1/?slaveok=true', ['readPreference' => 'nearest']],
    ['mongodb://127.0.0.1/?slaveok=false', ['readPreference' => 'nearest']],
    [null, ['readPreference' => 'nearest', 'slaveok' => true]],
    [null, ['readPreference' => 'nearest', 'slaveok' => true]],
    // Alternative values for true in URI string (all other strings are false)
    ['mongodb://127.0.0.1/?slaveok=t', []],
    ['mongodb://127.0.0.1/?slaveok=1', []],
    // Case insensitivity for URI string and array options
    ['mongodb://127.0.0.1/?slaveOk=True', []],
    ['mongodb://127.0.0.1/?SLAVEOK=TRUE', []],
    [null, ['slaveOk' => true]],
    [null, ['SLAVEOK' => true]],
];

foreach ($tests as $test) {
    list($uri, $options) = $test;

    $manager = new MongoDB\Driver\Manager($uri, $options);
    var_dump($manager->getReadPreference());
}

?>
===DONE===
--EXPECTF--
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(18) "secondaryPreferred"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "primary"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(18) "secondaryPreferred"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(18) "secondaryPreferred"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(18) "secondaryPreferred"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "nearest"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "nearest"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "nearest"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "nearest"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "nearest"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "nearest"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "nearest"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(18) "secondaryPreferred"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(18) "secondaryPreferred"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(18) "secondaryPreferred"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(18) "secondaryPreferred"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(18) "secondaryPreferred"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(18) "secondaryPreferred"
}
===DONE===
