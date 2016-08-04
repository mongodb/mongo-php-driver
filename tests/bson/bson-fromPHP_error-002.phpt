--TEST--o
BSON\fromPHP(): Encoding unknown Type objects as a document field value
--FILE--
<?php
use MongoDB\BSON as BSON;

require_once __DIR__ . '/../utils/tools.php';

class UnknownType implements BSON\Type {}

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
--EXPECTF--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Unexpected %SBSON\Type instance: UnknownType
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Unexpected %SBSON\Type instance: UnknownType
===DONE===
