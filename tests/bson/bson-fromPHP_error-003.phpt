--TEST--
MongoDB\BSON\fromPHP(): Encoding non-Serializable Type objects as a root element
--INI--
date.timezone=America/Los_Angeles
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class UnknownType implements MongoDB\BSON\Type {}

$tests = array(
    new UnknownType,
    new MongoDB\BSON\Binary('foobar', MongoDB\BSON\Binary::TYPE_GENERIC),
    new MongoDB\BSON\Javascript('function foo(bar) {var baz = bar; var bar = foo; return bar; }'),
    new MongoDB\BSON\MinKey,
    new MongoDB\BSON\MaxKey,
    new MongoDB\BSON\ObjectId,
    new MongoDB\BSON\Regex('regexp', 'i'),
    new MongoDB\BSON\Timestamp(1234, 5678),
    new MongoDB\BSON\UTCDateTime('1416445411987'),
);

foreach ($tests as $document) {
    echo throws(function() use ($document) {
        fromPHP($document);
    }, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
%S\BSON\Type instance UnknownType cannot be serialized as a root element
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
%S\BSON\Type instance %S\BSON\Binary cannot be serialized as a root element
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
%S\BSON\Type instance %S\BSON\Javascript cannot be serialized as a root element
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
%S\BSON\Type instance %S\BSON\MinKey cannot be serialized as a root element
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
%S\BSON\Type instance %S\BSON\MaxKey cannot be serialized as a root element
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
%S\BSON\Type instance %S\BSON\ObjectID cannot be serialized as a root element
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
%S\BSON\Type instance %S\BSON\Regex cannot be serialized as a root element
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
%S\BSON\Type instance %S\BSON\Timestamp cannot be serialized as a root element
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
%S\BSON\Type instance %S\BSON\UTCDateTime cannot be serialized as a root element
===DONE===
