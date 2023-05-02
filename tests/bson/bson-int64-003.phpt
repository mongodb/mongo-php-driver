--TEST--
MongoDB\BSON\Int64 encoded as 64-bit integer in BSON
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    ['int64' => new MongoDB\BSON\Int64('9223372036854775807')],
    ['int64' => new MongoDB\BSON\Int64('-9223372036854775808')],
    ['int64' => new MongoDB\BSON\Int64('2147483648')],
    ['int64' => new MongoDB\BSON\Int64('-2147483649')],
    ['int64' => new MongoDB\BSON\Int64(0)],
];

foreach($tests as $test) {
    $bson = fromPHP($test);
    hex_dump($bson);

    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
     0 : 14 00 00 00 12 69 6e 74 36 34 00 ff ff ff ff ff  [.....int64......]
    10 : ff ff 7f 00                                      [....]

     0 : 14 00 00 00 12 69 6e 74 36 34 00 00 00 00 00 00  [.....int64......]
    10 : 00 00 80 00                                      [....]

     0 : 14 00 00 00 12 69 6e 74 36 34 00 00 00 00 80 00  [.....int64......]
    10 : 00 00 00 00                                      [....]

     0 : 14 00 00 00 12 69 6e 74 36 34 00 ff ff ff 7f ff  [.....int64......]
    10 : ff ff ff 00                                      [....]

     0 : 14 00 00 00 12 69 6e 74 36 34 00 00 00 00 00 00  [.....int64......]
    10 : 00 00 00 00                                      [....]

===DONE===
