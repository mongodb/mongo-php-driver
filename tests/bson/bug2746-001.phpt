--TEST--
PHPC-2746: Non-string typeMap values are freed as interior pointers
--DESCRIPTION--
php_array_zval_to_string() converted a non-string zval to a temporary
zend_string and returned ZSTR_VAL() -- an interior pointer into that allocation
-- while reporting that the caller owned it, so the typeMap parser efree()'d a
pointer that was not an allocation base.

USE_ZEND_ALLOC=0 makes efree() use the system allocator, which rejects the
interior pointer at the call site instead of silently corrupting the Zend heap.

The value must convert to a non-interned string, so a multi-character number is
used: a single-character or empty result would be interned and take the
non-owning path.
--ENV--
USE_ZEND_ALLOC=0
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

/* Control: a string value takes the non-owning path */
echo throws(function () {
    MongoDB\BSON\Document::fromPHP(['x' => 1])->toPHP(['root' => 'no-such-class']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function () {
    MongoDB\BSON\Document::fromPHP(['x' => 1])->toPHP(['root' => 1234567890]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function () {
    MongoDB\BSON\PackedArray::fromPHP([1, 2])->toPHP(['root' => 1234567890]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class no-such-class does not exist
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class 1234567890 does not exist
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class 1234567890 does not exist
===DONE===
