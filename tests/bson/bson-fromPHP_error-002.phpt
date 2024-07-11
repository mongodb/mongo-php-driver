--TEST--
MongoDB\BSON\fromPHP(): Encoding unknown Type objects as a document field value
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

class UnknownType implements MongoDB\BSON\Type {}

$tests = array(
    array(new UnknownType()),
    array('x' => new UnknownType()),
);

foreach ($tests as $document) {
    echo throws(function() use ($document) {
        MongoDB\BSON\fromPHP($document);
    }, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Unexpected MongoDB\BSON\Type instance: UnknownType

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Unexpected MongoDB\BSON\Type instance: UnknownType
===DONE===
