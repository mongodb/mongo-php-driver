--TEST--
MongoDB\Driver\ReadPreference::__set_state()
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    [ 'mode' => 'primary' ],
    [ 'mode' => 'primaryPreferred' ],
    [ 'mode' => 'secondary' ],
    [ 'mode' => 'secondaryPreferred' ],
    [ 'mode' => 'nearest' ],
    [ 'mode' => 'secondary', 'tags' => [['dc' => 'ny']] ],
    [ 'mode' => 'secondary', 'tags' => [['dc' => 'ny'], ['dc' => 'sf', 'use' => 'reporting'], []] ],
    [ 'mode' => 'secondary', 'maxStalenessSeconds' => 1000 ],
    
];

foreach ($tests as $fields) {
    var_export(MongoDB\Driver\ReadPreference::__set_state($fields));
    echo "\n\n";
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
