--TEST--
MongoDB\BSON\UTCDateTime constructor requires an initialized DateTimeInterface
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

class MyDateTime extends DateTime
{
    public function __construct() {}
}

class MyDateTimeImmutable extends DateTimeImmutable
{
    public function __construct() {}
}

echo throws(
    fn () => new MongoDB\BSON\UTCDateTime(new MyDateTime()),
    MongoDB\Driver\Exception\InvalidArgumentException::class,
), PHP_EOL;

echo throws(
    fn () => new MongoDB\BSON\UTCDateTime(new MyDateTimeImmutable()),
    MongoDB\Driver\Exception\InvalidArgumentException::class,
), PHP_EOL;

$reflectionClass = new ReflectionClass(MyDateTime::class);

echo throws(
    fn () => new MongoDB\BSON\UTCDateTime($reflectionClass->newInstanceWithoutConstructor()),
    MongoDB\Driver\Exception\InvalidArgumentException::class,
), PHP_EOL;

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
The MyDateTime object has not been correctly initialized by its constructor
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
The MyDateTimeImmutable object has not been correctly initialized by its constructor
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
The MyDateTime object has not been correctly initialized by its constructor
===DONE===
