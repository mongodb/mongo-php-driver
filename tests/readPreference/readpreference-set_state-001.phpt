--TEST--
MongoDB\Driver\ReadPreference::__set_state()
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

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
MongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'primary',
))

MongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'primaryPreferred',
))

MongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'secondary',
))

MongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'secondaryPreferred',
))

MongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'nearest',
))

MongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'secondary',
   'tags' => 
  array (
    0 => 
    %Sarray(
       'dc' => 'ny',
    %S),
  ),
))

MongoDB\Driver\ReadPreference::__set_state(array(
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

MongoDB\Driver\ReadPreference::__set_state(array(
   'mode' => 'secondary',
   'maxStalenessSeconds' => 1000,
))

===DONE===
