--TEST--
MongoDB\BSON\fromPHP(): Encoding unknown Type objects as a document field value
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class UnknownType implements MongoDB\BSON\Type {}

$tests = array(
    array(new UnknownType()),
    array('x' => new UnknownType()),
);

foreach ($tests as $document) {
    echo throws(function() use ($document) {
        fromPHP($document);
    }, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Unexpected MongoDB\BSON\Type instance: UnknownType
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Unexpected MongoDB\BSON\Type instance: UnknownType
===DONE===
