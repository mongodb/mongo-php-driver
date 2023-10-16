--TEST--
MongoDB\BSON\Javascript unserialization expects optional scope to be array or object (Serializable interface)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('C:23:"MongoDB\BSON\Javascript":64:{a:2:{s:4:"code";s:13:"function() {}";s:5:"scope";s:7:"INVALID";}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected scope to be array or object, string given
===DONE===
