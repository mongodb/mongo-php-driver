--TEST--
MongoDB\Driver\ReadPreference construction with strings
--FILE--
<?php
$data = [
    "primary",
    "PrImAry",
    MongoDB\Driver\ReadPreference::PRIMARY,

    "primaryPreferred",
    "primarypreferred",
    MongoDB\Driver\ReadPreference::PRIMARY_PREFERRED,

    "secondary",
    "SEcOndArY",
    MongoDB\Driver\ReadPreference::SECONDARY,

    "secondaryPreferred",
    "secondarypreferred",
    MongoDB\Driver\ReadPreference::SECONDARY_PREFERRED,

    "nearest",
    "NeaRest",
    MongoDB\Driver\ReadPreference::NEAREST,
];

foreach ($data as $item) {
    try {
        $rp = new MongoDB\Driver\ReadPreference($item);
    } catch (\InvalidArgumentException $e) {
        echo $e->getMessage(), "\n";
    }
    var_dump( $rp );
}
?>
--EXPECTF--
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "primary"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "primary"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "primary"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(16) "primaryPreferred"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(16) "primaryPreferred"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(16) "primaryPreferred"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(18) "secondaryPreferred"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(18) "secondaryPreferred"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(18) "secondaryPreferred"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "nearest"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "nearest"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "nearest"
}
