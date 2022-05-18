--TEST--
MongoDB\Driver\Query::__construct(): comment option bsonSerialize() exception
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

class Comment implements MongoDB\BSON\Serializable
{
    public function bsonSerialize(): array
    {
        throw new Exception('php_phongo_zval_to_bson_value fails');
    }
}

echo throws(function() {
    new MongoDB\Driver\Query([], ['comment' => new Comment()]);
}, Exception::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got Exception
php_phongo_zval_to_bson_value fails
===DONE===
