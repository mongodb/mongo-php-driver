--TEST--
MongoDB\BSON\Javascript unserialization expects optional scope to be array or object
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    unserialize('C:23:"MongoDB\BSON\Javascript":68:{a:2:{s:4:"code";s:17:"function foo() {}";s:5:"scope";s:7:"INVALID";}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected scope to be array or object, string given
===DONE===
