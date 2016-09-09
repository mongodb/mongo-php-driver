--TEST--
MongoDB\BSON\UTCDateTime serialization (unserialize 32-bit data on 64-bit)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

$tests = [
    '0',
    '-1416445411987',
    '1416445411987',
];

foreach ($tests as $milliseconds) {
    $s = sprintf('C:24:"MongoDB\BSON\UTCDateTime":%d:{a:1:{s:12:"milliseconds";s:%d:"%s";}}', 32 + strlen($milliseconds) + strlen(strlen($milliseconds)), strlen($milliseconds), $milliseconds);

    var_dump($s);
    var_dump(unserialize($s));
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
string(71) "C:24:"MongoDB\BSON\UTCDateTime":34:{a:1:{s:12:"milliseconds";s:1:"0";}}"
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(1) "0"
}

string(85) "C:24:"MongoDB\BSON\UTCDateTime":48:{a:1:{s:12:"milliseconds";s:14:"-1416445411987";}}"
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(14) "-1416445411987"
}

string(84) "C:24:"MongoDB\BSON\UTCDateTime":47:{a:1:{s:12:"milliseconds";s:13:"1416445411987";}}"
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(13) "1416445411987"
}

===DONE===
