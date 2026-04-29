--TEST--
MongoDB\Driver\ReadPreference can be cloned
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$readPreference = new MongoDB\Driver\ReadPreference(
    MongoDB\Driver\ReadPreference::SECONDARY,
    [['dc' => 'ny']],
    ['maxStalenessSeconds' => 1000]
);
$clone = clone $readPreference;

var_dump($clone == $readPreference);
var_dump($clone === $readPreference);

unset($readPreference);

var_dump($clone);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  array(1) {
    [0]=>
    object(stdClass)#%d (%d) {
      ["dc"]=>
      string(2) "ny"
    }
  }
  ["maxStalenessSeconds"]=>
  int(1000)
  ["hedge"]=>
  NULL
}
===DONE===
