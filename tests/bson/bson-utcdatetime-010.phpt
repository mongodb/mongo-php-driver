--TEST--
MongoDB\BSON\UTCDateTime construction from DateTime and DateTimeImmutable subclasses
--INI--
date.timezone=UTC
--FILE--
<?php

class MyDateTime extends DateTime
{
    public function __construct()
    {
        parent::__construct('2011-01-01T15:03:01.012345Z');
    }
}

class MyDateTimeImmutable extends DateTimeImmutable
{
    public function __construct()
    {
        parent::__construct('2011-01-01T15:03:01.012345Z');
    }
}

var_dump(new MongoDB\BSON\UTCDateTime(new MyDateTime()));
var_dump(new MongoDB\BSON\UTCDateTime(new MyDateTimeImmutable()));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(13) "1293894181012"
}
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(13) "1293894181012"
}
===DONE===
