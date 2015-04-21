--TEST--
Disable serialization of objects
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE)?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$str = 'O:22:"MongoDB\Driver\Manager":0:{}';
try {
    /* We throw exception in 5.3 - 5.6 triggers warning and returns false
     * This is inconsistency is only with manipulated serialization strings, or using them across versions
     * which isn't support by PHP in the getgo anyway */
    $v = @unserialize($str);
    var_dump($v);
} catch(Exception $e) {
    var_dump(false);
}

$manager = new MongoDB\Driver\Manager(STANDALONE);
throws(function() use ($manager) {
    serialize($manager);
}, "Exception");
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(false)
OK: Got Exception
===DONE===
