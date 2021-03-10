--TEST--
PHPC-1698: php_phongo_read_preference_prep_tagsets may leak in convert_to_object
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$args = ['mode' => 'secondary', 'tags' => [['dc' => 'ny']]];
var_dump(MongoDB\Driver\ReadPreference::__set_state($args));
var_dump($args);

$tagSets = [['dc' => 'ny']];
var_dump(new MongoDB\Driver\ReadPreference('secondary', $tagSets));
var_dump($tagSets);

$uriTagSets = [['dc' => 'ny']];
var_dump((create_test_manager(null, ['readPreference' => 'secondary', 'readPreferenceTags' => $uriTagSets]))->getReadPreference());
var_dump($uriTagSets);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\ReadPreference)#%d (2) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  array(1) {
    [0]=>
    object(stdClass)#%d (1) {
      ["dc"]=>
      string(2) "ny"
    }
  }
}
array(2) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  array(1) {
    [0]=>
    array(1) {
      ["dc"]=>
      string(2) "ny"
    }
  }
}
object(MongoDB\Driver\ReadPreference)#%d (2) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  array(1) {
    [0]=>
    object(stdClass)#%d (1) {
      ["dc"]=>
      string(2) "ny"
    }
  }
}
array(1) {
  [0]=>
  array(1) {
    ["dc"]=>
    string(2) "ny"
  }
}
object(MongoDB\Driver\ReadPreference)#%d (2) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  array(1) {
    [0]=>
    object(stdClass)#%d (1) {
      ["dc"]=>
      string(2) "ny"
    }
  }
}
array(1) {
  [0]=>
  array(1) {
    ["dc"]=>
    string(2) "ny"
  }
}
===DONE===
