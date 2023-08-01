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
    %r\(object\)? %rarray(
       'dc' => 'ny',
    %r\)?%r),
  ),
))

%r\\?%rMongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'secondary',
   'tags' =>%w
  array (
    0 =>%w
    %r\(object\)? %rarray(
       'dc' => 'ny',
    %r\)?%r),
    1 =>%w
    %r\(object\)? %rarray(
       'dc' => 'sf',
       'use' => 'reporting',
    %r\)?%r),
    2 =>%w
    %r\(object\)? %rarray(
    %r\)?%r),
  ),
))

%r\\?%rMongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'secondary',
   'maxStalenessSeconds' => 1000,
))

===DONE===
