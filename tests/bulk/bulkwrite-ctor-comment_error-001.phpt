--TEST--
MongoDB\Driver\BulkWrite::__construct(): comment option bsonSerialize() exception
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

class Comment implements MongoDB\BSON\Serializable
{
    #[\ReturnTypeWillChange]
    public function bsonSerialize(): array
    {
        throw new Exception('phongo_zval_to_bson_value fails');
    }
}

echo throws(function() {
    new MongoDB\Driver\BulkWrite(['comment' => new Comment()]);
}, Exception::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got Exception
phongo_zval_to_bson_value fails
===DONE===
