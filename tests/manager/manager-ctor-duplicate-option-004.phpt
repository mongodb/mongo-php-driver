--TEST--
MongoDB\Driver\Manager::__construct() with duplicate read preference tags
--FILE--
<?php

$manager = new MongoDB\Driver\Manager(null, ['readPreference' => 'secondary', 'readPreferenceTags' => [['dc' => 'ny']], 'readpreferencetags' => [['dc' => 'ca']]]);

var_dump($manager->getReadPreference()->getTagSets());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(1) {
  [0]=>
  array(1) {
    ["dc"]=>
    string(2) "ca"
  }
}
===DONE===
