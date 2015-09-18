--TEST--
BSON\fromPHP(): Encoding non-Serializable Type objects as a root element
--INI--
date.timezone=America/Los_Angeles
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
use MongoDB\BSON as BSON;

require_once __DIR__ . "/../utils/basic.inc";

class UnknownType implements BSON\Type {}

$tests = array(
    new UnknownType,
    new BSON\Binary('foobar', BSON\Binary::TYPE_GENERIC),
    new BSON\Javascript('function foo(bar) {var baz = bar; var bar = foo; return bar; }'),
    new BSON\MinKey,
    new BSON\MaxKey,
    new BSON\ObjectId,
    new BSON\Regex('regexp', 'i'),
    new BSON\Timestamp(1234, 5678),
    new BSON\UTCDateTime('1416445411987'),
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
