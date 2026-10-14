--TEST--
MongoDB\Driver\ReadPreference: var_export()
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY_PREFERRED),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY_PREFERRED),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::NEAREST),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY, []),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'ny']]),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'ny'], ['dc' => 'sf', 'use' => 'reporting'], []]),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['maxStalenessSeconds' => 1000]),
];

foreach ($tests as $test) {
    echo var_export($test, true), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%r\\?%rMongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'primary',
   'tags' => NULL,
   'maxStalenessSeconds' => -1,
   'hedge' => NULL,
))
%r\\?%rMongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'primaryPreferred',
   'tags' => NULL,
   'maxStalenessSeconds' => -1,
   'hedge' => NULL,
))
%r\\?%rMongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'secondary',
   'tags' => NULL,
   'maxStalenessSeconds' => -1,
   'hedge' => NULL,
))
%r\\?%rMongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'secondaryPreferred',
   'tags' => NULL,
   'maxStalenessSeconds' => -1,
   'hedge' => NULL,
))
%r\\?%rMongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'nearest',
   'tags' => NULL,
   'maxStalenessSeconds' => -1,
   'hedge' => NULL,
))
%r\\?%rMongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'primary',
   'tags' => NULL,
   'maxStalenessSeconds' => -1,
   'hedge' => NULL,
))
%r\\?%rMongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'secondary',
   'tags' =>%w
  array (
    0 =>%w
    (object) array(
       'dc' => 'ny',
    ),
  ),
   'maxStalenessSeconds' => -1,
   'hedge' => NULL,
))
%r\\?%rMongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'secondary',
   'tags' =>%w
  array (
    0 =>%w
    (object) array(
       'dc' => 'ny',
    ),
    1 =>%w
    (object) array(
       'dc' => 'sf',
       'use' => 'reporting',
    ),
    2 =>%w
    (object) array(
    ),
  ),
   'maxStalenessSeconds' => -1,
   'hedge' => NULL,
))
%r\\?%rMongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'secondary',
   'tags' => NULL,
   'maxStalenessSeconds' => 1000,
   'hedge' => NULL,
))
===DONE===
