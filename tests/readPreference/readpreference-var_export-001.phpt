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
   'tags' => 
  array (
    0 => 
    %Sarray(
       'dc' => 'ny',
    %S),
  ),
))
%r\\?%rMongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'secondary',
   'tags' => 
  array (
    0 => 
    %Sarray(
       'dc' => 'ny',
    %S),
    1 => 
    %Sarray(
       'dc' => 'sf',
       'use' => 'reporting',
    %S),
    2 => 
    %Sarray(
    %S),
  ),
))
%r\\?%rMongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'secondary',
   'maxStalenessSeconds' => 1000,
))
===DONE===
