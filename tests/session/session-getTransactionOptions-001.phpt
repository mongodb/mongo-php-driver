--TEST--
MongoDB\Driver\Session::getTransactionOptions()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_no_transactions(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
$session = $manager->startSession();

var_dump($session->getTransactionOptions());

$options = [
    ['maxCommitTimeMS' => 0],
    ['maxCommitTimeMS' => 1],
    ['readConcern' => new \MongoDB\Driver\ReadConcern('majority')],
    ['readPreference' => new \MongoDB\Driver\ReadPreference('primaryPreferred')],
    ['writeConcern' => new \MongoDB\Driver\WriteConcern('majority')],
];

foreach ($options as $test) {
    // Session no longer needs to be restarted once CDRIVER-3366 is fixed
    $session = $manager->startSession();

    $session->startTransaction($test);

    var_dump($session->getTransactionOptions());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
NULL
array(1) {
  ["readPreference"]=>
  object(MongoDB\Driver\ReadPreference)#%d (1) {
    ["mode"]=>
    string(7) "primary"
  }
}
array(2) {
  ["maxCommitTimeMS"]=>
  int(1)
  ["readPreference"]=>
  object(MongoDB\Driver\ReadPreference)#%d (1) {
    ["mode"]=>
    string(7) "primary"
  }
}
array(2) {
  ["readConcern"]=>
  object(MongoDB\Driver\ReadConcern)#%d (1) {
    ["level"]=>
    string(8) "majority"
  }
  ["readPreference"]=>
  object(MongoDB\Driver\ReadPreference)#%d (1) {
    ["mode"]=>
    string(7) "primary"
  }
}
array(1) {
  ["readPreference"]=>
  object(MongoDB\Driver\ReadPreference)#%d (1) {
    ["mode"]=>
    string(16) "primaryPreferred"
  }
}
array(2) {
  ["readPreference"]=>
  object(MongoDB\Driver\ReadPreference)#%d (1) {
    ["mode"]=>
    string(7) "primary"
  }
  ["writeConcern"]=>
  object(MongoDB\Driver\WriteConcern)#%d (1) {
    ["w"]=>
    string(8) "majority"
  }
}
===DONE===
