--TEST--
MongoDB\BSON\Timestamp::getIncrement()
--FILE--
<?php

$tests = [
    new MongoDB\BSON\Timestamp(1234, 5678),
    new MongoDB\BSON\Timestamp(2147483647, 0),
    new MongoDB\BSON\Timestamp(0, 2147483647),
];

foreach ($tests as $test) {
    printf("Test %s\n", $test);
    var_dump($test->getIncrement());
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Test [1234:5678]
int(1234)

Test [2147483647:0]
int(2147483647)

Test [0:2147483647]
int(0)

===DONE===
