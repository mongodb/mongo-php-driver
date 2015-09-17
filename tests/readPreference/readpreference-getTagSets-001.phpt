--TEST--
MongoDB\Driver\ReadPreference::getMode()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$tests = array(
    null,
    array(),
    array(array('dc' => 'ny'), array()),
    array(array('dc' => 'ny'), array('dc' => 'sf', 'use' => 'reporting'), array()),
);

foreach ($tests as $test) {
    $rp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY_PREFERRED, $test);
    var_dump($rp->getTagSets());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(0) {
}
array(0) {
}
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
===DONE===
