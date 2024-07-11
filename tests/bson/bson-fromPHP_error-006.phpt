--TEST--
MongoDB\BSON\fromPHP(): PHP documents with null bytes in field name
--DESCRIPTION--
BSON Corpus spec prose test #1
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo "\nTesting array with one leading null byte in field name\n";
echo throws(function() {
    MongoDB\BSON\fromPHP(["\0" => 1]);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

echo "\nTesting array with one trailing null byte in field name\n";
echo throws(function() {
    MongoDB\BSON\fromPHP(["a\0" => 1]);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

echo "\nTesting array with multiple null bytes in field name\n";
echo throws(function() {
    MongoDB\BSON\fromPHP(["\0\0\0" => 1]);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

echo "\nTesting object with one trailing null byte in field name\n";
echo throws(function() {
    MongoDB\BSON\fromPHP((object) ["a\0" => 1]);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

echo "\nTesting nested array with one trailing null byte in field name\n";
echo throws(function() {
    MongoDB\BSON\fromPHP(['a' => ["b\0" => 1]]);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing array with one leading null byte in field name

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
BSON keys cannot contain null bytes. Unexpected null byte after "".

Testing array with one trailing null byte in field name

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
BSON keys cannot contain null bytes. Unexpected null byte after "a".

Testing array with multiple null bytes in field name

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
BSON keys cannot contain null bytes. Unexpected null byte after "".

Testing object with one trailing null byte in field name

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
BSON keys cannot contain null bytes. Unexpected null byte after "a".

Testing nested array with one trailing null byte in field name

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
BSON keys cannot contain null bytes. Unexpected null byte after "b".
===DONE===
