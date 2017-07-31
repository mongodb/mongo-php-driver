--TEST--
MongoDB\BSON\Timestamp::getTimestamp()
--FILE--
<?php

$tests = [
    new MongoDB\BSON\Timestamp(1234, 5678),
    new MongoDB\BSON\Timestamp(2147483647, 0),
    new MongoDB\BSON\Timestamp(0, 2147483647),
];

foreach ($tests as $test) {
    printf("Test %s\n", $test);
    var_dump($test->getTimestamp());
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Test [1234:5678]
int(5678)

Test [2147483647:0]
int(0)

Test [0:2147483647]
int(2147483647)

===DONE===
