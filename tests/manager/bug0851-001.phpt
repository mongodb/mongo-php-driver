--TEST--
PHPC-851: Manager constructor should not modify options argument
--FILE--
<?php

$options = [
    'readPreference' => 'secondaryPreferred',
    'readPreferenceTags' => [
        ['dc' => 'ny'],
        [],
    ],
];

$manager = new MongoDB\Driver\Manager(null, $options);
var_dump($options);

/* Dump the Manager's ReadPreference to ensure that each element in the
 * readPreferenceTags option was converted to an object. */
var_dump($manager->getReadPreference());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(2) {
  ["readPreference"]=>
  string(18) "secondaryPreferred"
  ["readPreferenceTags"]=>
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
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(18) "secondaryPreferred"
  ["tags"]=>
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
}
===DONE===
