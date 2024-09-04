--TEST--
MongoDB\BSON\UTCDateTime::toDateTimeImmutable() with dates before the Unix epoch
--INI--
date.timezone=UTC
--FILE--
<?php

$dates = [
    '1960-01-01 12:12:12.1',
    '1969-12-31 23:59:59.999',
];

foreach ($dates as $date) {
    $dateTime = new \DateTimeImmutable($date);
    echo $dateTime->format(DateTimeInterface::RFC3339_EXTENDED), PHP_EOL;

    $utcDateTime = new MongoDB\BSON\UTCDateTime($dateTime);

    $newDate = $utcDateTime->toDateTimeImmutable();
    echo $newDate->format(DateTimeInterface::RFC3339_EXTENDED), PHP_EOL, PHP_EOL;
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
1960-01-01T12:12:12.100+00:00
1960-01-01T12:12:12.100+00:00

1969-12-31T23:59:59.999+00:00
1969-12-31T23:59:59.999+00:00

===DONE===
