--TEST--
MongoDB\BSON\UTCDateTimeInterface example with MongoDB\BSON\TypeWrapper
--INI--
date.timezone=UTC
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class MyUTCDateTime implements MongoDB\BSON\TypeWrapper, MongoDB\BSON\UTCDateTimeInterface
{
    private $date;

    public function __construct($milliseconds)
    {
        $this->date = new MongoDB\BSON\UTCDateTime($milliseconds);
    }

    public static function createFromBSONType(MongoDB\BSON\Type $type)
    {
        if ( ! $type instanceof MongoDB\BSON\UTCDateTime) {
            throw new InvalidArgumentException('Cannot create MyUTCDateTime from ' . get_class($type));
        }

        return new self((string) $type);
    }

    public function toBSONType()
    {
        return $this->date;
    }

    public function toDateTime()
    {
        return $this->date->toDateTime();
    }

    public function __toString()
    {
        return (string) $this->date;
    }
}

$date = new MongoDB\BSON\UTCDateTime('1416445411987');
$myDate = new MyUTCDateTime('1416445411987');

echo "\nTesting MyUTCDateTime and UTCDateTime are equivalent\n";
// Do not use "===" since each scope is a different DateTime instance
var_dump($date->toDateTime() == $myDate->toDateTime());
var_dump((string) $date === (string) $myDate);

echo "\nTesting MyUTCDateTime and UTCDateTime produce the same BSON\n";
var_dump(fromPHP(['x' => $date]) === fromPHP(['x' => $myDate]));

echo "\nTesting UTCDateTime to BSON to MyUTCDateTime\n";
var_dump(toPHP(fromPHP(['x' => $date]), ['types' => ['UTCDateTime' => 'MyUTCDateTime']])->x);

echo "\nTesting MyUTCDateTime to BSON to MyUTCDateTime\n";
var_dump(toPHP(fromPHP(['x' => $myDate]), ['types' => ['UTCDateTime' => 'MyUTCDateTime']])->x);

echo "\nTesting MyUTCDateTime to BSON to UTCDateTime\n";
var_dump(toPHP(fromPHP(['x' => $myDate]))->x);

echo "\nTesting MyUTCDateTime::createFromBSONType() expects UTCDateTime\n";
echo throws(function() {
    toPHP(fromPHP(['x' => new MongoDB\BSON\MinKey]), ['types' => ['MinKey' => 'MyUTCDateTime']]);
}, 'InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing MyUTCDateTime and UTCDateTime are equivalent
bool(true)
bool(true)

Testing MyUTCDateTime and UTCDateTime produce the same BSON
bool(true)

Testing UTCDateTime to BSON to MyUTCDateTime
object(MyUTCDateTime)#%d (%d) {
  ["date":"MyUTCDateTime":private]=>
  object(MongoDB\BSON\UTCDateTime)#%d (%d) {
    ["milliseconds"]=>
    string(13) "1416445411987"
  }
}

Testing MyUTCDateTime to BSON to MyUTCDateTime
object(MyUTCDateTime)#%d (%d) {
  ["date":"MyUTCDateTime":private]=>
  object(MongoDB\BSON\UTCDateTime)#%d (%d) {
    ["milliseconds"]=>
    string(13) "1416445411987"
  }
}

Testing MyUTCDateTime to BSON to UTCDateTime
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(13) "1416445411987"
}

Testing MyUTCDateTime::createFromBSONType() expects UTCDateTime
OK: Got InvalidArgumentException
Cannot create MyUTCDateTime from MongoDB\BSON\MinKey
===DONE===
