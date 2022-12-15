--TEST--
MongoDB\BSON\PackedArray::unserialize() throws on errors
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '7.4.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    'Empty string' => '',
    'Last byte is non-zero' => pack(
        'VCa*xVa*',
        4 + 1 + 2 + 4 + 2, // Size: 4 bytes for size, 1 byte for type, 2 bytes for key, 4 bytes for string length, 2 bytes for string
        0x02,
        '0',
        2,
        'a'
    ),
    'Length does not match document length' => pack('VCa*Va*xx', 7, 0x02, '0', 1, 'a'),
];

foreach ($tests as $test) {
    echo throws(function() use ($test) {
        $data = base64_encode($test);
        $dataLength = strlen($data);

        $payload = sprintf(
            'C:%d:"%s":%d:{a:1:{s:4:"data";s:%d:"%s";}}',
            strlen(MongoDB\BSON\PackedArray::class),
            MongoDB\BSON\PackedArray::class,
            $dataLength + 23 + strlen((string) $dataLength), // 23 characters for everything besides the number of bytes in the payload
            $dataLength,
            $data
        );

        unserialize($payload);
    }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\PackedArray initialization requires valid BSON
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\PackedArray initialization requires valid BSON
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\PackedArray initialization requires valid BSON
===DONE===
