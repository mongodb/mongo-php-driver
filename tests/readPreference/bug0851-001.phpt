--TEST--
PHPC-851: ReadPreference constructor should not modify tagSets argument
--FILE--
<?php

$tagSets = [
    ['dc' => 'ny'],
    [],
];

$rp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY_PREFERRED, $tagSets);
var_dump($tagSets);

/* Dump the Manager's ReadPreference to ensure that each element in the $tagSets
 * argument was converted to an object. */
var_dump($rp);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
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
