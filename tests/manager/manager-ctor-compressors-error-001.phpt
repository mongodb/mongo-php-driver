--TEST--
MongoDB\Driver\Manager::__construct(): invalid "compressors" URI option values
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    'Comma within element' => ['zlib,zstd'],
    'Comma within element alongside other elements' => ['zlib', 'zstd,noop'],
    'Embedded NUL in string' => "zlib\0ignored,zstd",
    'Embedded NUL in element' => ["zlib\0ignored", 'zstd'],
    'String keys' => ['a' => 'zstd'],
    'Non-zero integer key' => [1 => 'zstd'],
    'Gapped list' => [0 => 'zstd', 2 => 'zlib'],
    'Mixed keys' => ['zstd', 'a' => 'zlib'],
];

foreach ($tests as $name => $compressors) {
    printf("%s:\n", $name);

    echo throws(function() use ($compressors) {
        create_test_manager(null, ['compressors' => $compressors]);
    }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Comma within element:
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Compressor names cannot contain commas
Comma within element alongside other elements:
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Compressor names cannot contain commas
Embedded NUL in string:
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
"compressors" URI option cannot contain null bytes
Embedded NUL in element:
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
"compressors" URI option cannot contain null bytes
String keys:
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "compressors" URI option to be a list
Non-zero integer key:
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "compressors" URI option to be a list
Gapped list:
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "compressors" URI option to be a list
Mixed keys:
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "compressors" URI option to be a list
===DONE===
