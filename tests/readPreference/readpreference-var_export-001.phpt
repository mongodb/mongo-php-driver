--TEST--
MongoDB\Driver\ReadPreference: var_export()
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY_PREFERRED),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY_PREFERRED),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_NEAREST),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY, []),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, [['dc' => 'ny']]),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, [['dc' => 'ny'], ['dc' => 'sf', 'use' => 'reporting'], []]),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, null, ['maxStalenessSeconds' => 1000]),
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
))
%r\\?%rMongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'primaryPreferred',
))
%r\\?%rMongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'secondary',
))
%r\\?%rMongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'secondaryPreferred',
))
%r\\?%rMongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'nearest',
))
%r\\?%rMongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'primary',
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
))
%r\\?%rMongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'secondary',
   'maxStalenessSeconds' => 1000,
))
===DONE===
