--TEST--
MongoDB\Driver\Manager::getReadPreference()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$tests = array(
    array(STANDALONE, array()),
    array(STANDALONE . '/?readPreference=secondary', array()),
    array(STANDALONE, array('readPreference' => 'primaryPreferred')),
    array(STANDALONE . '/?readPreference=secondary', array('readPreference' => 'secondaryPreferred')),
    array(STANDALONE . '/?readPreference=secondary&readPreferenceTags=dc:ny,use:reports&readPreferenceTags=', array()),
    array(STANDALONE . '/?readPreference=secondary', array('readPreferenceTags' => array(array('dc' => 'ny', 'use' => 'reports'), array()))),
    array(STANDALONE . '/?readPreference=secondary&readPreferenceTags=dc:ny,use:reports', array('readPreferenceTags' => array(array('dc' => 'ca')))),
);

foreach ($tests as $i => $test) {
    list($uri, $options) = $test;

    $manager = new MongoDB\Driver\Manager($uri, $options);
    var_dump($manager->getReadPreference());

    // Test for !return_value_used
    $manager->getReadPreference();
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  int(1)
  ["tags"]=>
  array(0) {
  }
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  int(2)
  ["tags"]=>
  array(0) {
  }
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  int(5)
  ["tags"]=>
  array(0) {
  }
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  int(6)
  ["tags"]=>
  array(0) {
  }
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  int(2)
  ["tags"]=>
  array(2) {
    [0]=>
    array(2) {
      ["dc"]=>
      string(2) "ny"
      ["use"]=>
      string(7) "reports"
    }
    [1]=>
    array(0) {
    }
  }
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  int(2)
  ["tags"]=>
  array(2) {
    [0]=>
    array(2) {
      ["dc"]=>
      string(2) "ny"
      ["use"]=>
      string(7) "reports"
    }
    [1]=>
    array(0) {
    }
  }
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  int(2)
  ["tags"]=>
  array(1) {
    [0]=>
    array(1) {
      ["dc"]=>
      string(2) "ca"
    }
  }
}
===DONE===
