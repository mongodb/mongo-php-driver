--TEST--
MongoDB\Driver\ReadPreference::getTagSets() with string mode
--FILE--
<?php

$tests = [
    null,
    [],
    [['dc' => 'ny'], []],
    [['dc' => 'ny'], ['dc' => 'sf', 'use' => 'reporting'], []],
];

foreach ($tests as $test) {
    $rp = new MongoDB\Driver\ReadPreference("secondaryPreferred", $test);
    var_dump($rp->getTagSets());
    var_dump($rp->tags);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(0) {
}
NULL
array(0) {
}
NULL
array(2) {
  [0]=>
  array(1) {
    ["dc"]=>
    string(2) "ny"
  }
  [1]=>
  array(0) {
  }
}
array(2) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["dc"]=>
    string(2) "ny"
  }
  [1]=>
  object(stdClass)#%d (%d) {
  }
}
array(3) {
  [0]=>
  array(1) {
    ["dc"]=>
    string(2) "ny"
  }
  [1]=>
  array(2) {
    ["dc"]=>
    string(2) "sf"
    ["use"]=>
    string(9) "reporting"
  }
  [2]=>
  array(0) {
  }
}
array(3) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["dc"]=>
    string(2) "ny"
  }
  [1]=>
  object(stdClass)#%d (%d) {
    ["dc"]=>
    string(2) "sf"
    ["use"]=>
    string(9) "reporting"
  }
  [2]=>
  object(stdClass)#%d (%d) {
  }
}
===DONE===
