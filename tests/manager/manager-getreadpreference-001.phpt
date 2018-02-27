--TEST--
MongoDB\Driver\Manager::getReadPreference()
--FILE--
<?php

$tests = array(
    array(null, array()),
    array('mongodb://127.0.0.1/?readPreference=secondary', array()),
    array(null, array('readPreference' => 'primaryPreferred')),
    array('mongodb://127.0.0.1/?readPreference=secondary', array('readPreference' => 'secondaryPreferred')),
    array('mongodb://127.0.0.1/?readPreference=secondary&readPreferenceTags=dc:ny,use:reports&readPreferenceTags=', array()),
    array('mongodb://127.0.0.1/?readPreference=secondary', array('readPreferenceTags' => array(array('dc' => 'ny', 'use' => 'reports'), array()))),
    array('mongodb://127.0.0.1/?readPreference=secondary&readPreferenceTags=dc:ny,use:reports', array('readPreferenceTags' => array(array('dc' => 'ca')))),
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
  string(7) "primary"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(16) "primaryPreferred"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(18) "secondaryPreferred"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  array(2) {
    [0]=>
    object(stdClass)#%d (%d) {
      ["dc"]=>
      string(2) "ny"
      ["use"]=>
      string(7) "reports"
    }
    [1]=>
    object(stdClass)#%d (%d) {
    }
  }
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  array(2) {
    [0]=>
    object(stdClass)#%d (%d) {
      ["dc"]=>
      string(2) "ny"
      ["use"]=>
      string(7) "reports"
    }
    [1]=>
    object(stdClass)#%d (%d) {
    }
  }
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  array(1) {
    [0]=>
    object(stdClass)#%d (%d) {
      ["dc"]=>
      string(2) "ca"
    }
  }
}
===DONE===
