--TEST--
MongoDB\BSON\Timestamp can be cloned
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$timestamp = new MongoDB\BSON\Timestamp(1234, 5678);
$timestamp->foo = 'bar';

$clone = clone $timestamp;

var_dump($clone == $timestamp);
var_dump($clone === $timestamp);

unset($timestamp);

var_dump($clone);
var_dump($clone->foo);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
object(MongoDB\BSON\Timestamp)#%d (2) {
  ["increment"]=>
  string(4) "1234"
  ["timestamp"]=>
  string(4) "5678"
}
string(3) "bar"
===DONE===
