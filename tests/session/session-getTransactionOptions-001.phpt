--TEST--
MongoDB\Driver\Session::getTransactionOptions()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_no_transactions(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$session = $manager->startSession();

var_dump($session->getTransactionOptions());

$options = [
    ['maxCommitTimeMS' => 0],
    ['maxCommitTimeMS' => 1],
    ['readConcern' => new \MongoDB\Driver\ReadConcern('majority')],
    ['readPreference' => new \MongoDB\Driver\ReadPreference('primaryPreferred')],
    ['writeConcern' => new \MongoDB\Driver\WriteConcern('majority')],
    [
        'maxCommitTimeMS' => 5000,
        'readConcern' => new \MongoDB\Driver\ReadConcern(\MongoDB\Driver\ReadConcern::MAJORITY),
        'readPreference' => new \MongoDB\Driver\ReadPreference(\MongoDB\Driver\ReadPreference::PRIMARY),
        'writeConcern' => new \MongoDB\Driver\WriteConcern(\MongoDB\Driver\WriteConcern::MAJORITY),
    ],
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
  object(MongoDB\Driver\ReadPreference)#%d (%d) {
    ["mode"]=>
    string(7) "primary"
    ["tags"]=>
    NULL
    ["maxStalenessSeconds"]=>
    int(-1)
    ["hedge"]=>
    NULL
  }
}
array(2) {
  ["maxCommitTimeMS"]=>
  int(1)
  ["readPreference"]=>
  object(MongoDB\Driver\ReadPreference)#%d (%d) {
    ["mode"]=>
    string(7) "primary"
    ["tags"]=>
    NULL
    ["maxStalenessSeconds"]=>
    int(-1)
    ["hedge"]=>
    NULL
  }
}
array(2) {
  ["readConcern"]=>
  object(MongoDB\Driver\ReadConcern)#%d (1) {
    ["level"]=>
    string(8) "majority"
  }
  ["readPreference"]=>
  object(MongoDB\Driver\ReadPreference)#%d (%d) {
    ["mode"]=>
    string(7) "primary"
    ["tags"]=>
    NULL
    ["maxStalenessSeconds"]=>
    int(-1)
    ["hedge"]=>
    NULL
  }
}
array(1) {
  ["readPreference"]=>
  object(MongoDB\Driver\ReadPreference)#%d (%d) {
    ["mode"]=>
    string(16) "primaryPreferred"
    ["tags"]=>
    NULL
    ["maxStalenessSeconds"]=>
    int(-1)
    ["hedge"]=>
    NULL
  }
}
array(2) {
  ["readPreference"]=>
  object(MongoDB\Driver\ReadPreference)#%d (%d) {
    ["mode"]=>
    string(7) "primary"
    ["tags"]=>
    NULL
    ["maxStalenessSeconds"]=>
    int(-1)
    ["hedge"]=>
    NULL
  }
  ["writeConcern"]=>
  object(MongoDB\Driver\WriteConcern)#%d (3) {
    ["w"]=>
    string(8) "majority"
    ["j"]=>
    NULL
    ["wtimeout"]=>
    int(0)
  }
}
array(4) {
  ["maxCommitTimeMS"]=>
  int(5000)
  ["readConcern"]=>
  object(MongoDB\Driver\ReadConcern)#%d (1) {
    ["level"]=>
    string(8) "majority"
  }
  ["readPreference"]=>
  object(MongoDB\Driver\ReadPreference)#%d (%d) {
    ["mode"]=>
    string(7) "primary"
    ["tags"]=>
    NULL
    ["maxStalenessSeconds"]=>
    int(-1)
    ["hedge"]=>
    NULL
  }
  ["writeConcern"]=>
  object(MongoDB\Driver\WriteConcern)#%d (3) {
    ["w"]=>
    string(8) "majority"
    ["j"]=>
    NULL
    ["wtimeout"]=>
    int(0)
  }
}
===DONE===
