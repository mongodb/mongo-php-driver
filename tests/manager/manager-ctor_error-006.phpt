--TEST--
PHPC-2804: MongoDB\Driver\Manager::__construct() parse errors do not include the connection string
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$tests = [
    'mongodb://user:SENTINELpassword@localhost:99999/',
    'mongodb://user:SENTINELpassword@localhost/?connectTimeoutMS=invalid',
    'mongodb://user:SENTINELpassword@localhost/db%',
    'mongodb://user:SENTINEL/password@localhost/',
];

foreach ($tests as $uri) {
    try {
        new MongoDB\Driver\Manager($uri);
        echo "No exception thrown\n";
    } catch (MongoDB\Driver\Exception\InvalidArgumentException $e) {
        echo get_class($e), "\n";
        var_dump(str_starts_with($e->getMessage(), 'Failed to parse MongoDB URI: '));
        var_dump(stripos($e->getMessage(), 'SENTINEL'));
    }
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
MongoDB\Driver\Exception\InvalidArgumentException
bool(true)
bool(false)
MongoDB\Driver\Exception\InvalidArgumentException
bool(true)
bool(false)
MongoDB\Driver\Exception\InvalidArgumentException
bool(true)
bool(false)
MongoDB\Driver\Exception\InvalidArgumentException
bool(true)
bool(false)
===DONE===
