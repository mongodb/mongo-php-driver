--TEST--
MongoDB\Driver\Manager::__construct(): compressors option accepts an array of strings
--FILE--
<?php

class CompressorLogger implements MongoDB\Driver\Monitoring\LogSubscriber
{
    public function log(int $level, string $domain, string $message): void
    {
        if (
            $level === MongoDB\Driver\Monitoring\LogSubscriber::LEVEL_WARNING
            && str_starts_with($message, 'Unsupported compressor')
        ) {
            echo "Unsupported compressor\n";
        }
    }
}

MongoDB\Driver\Monitoring\addSubscriber(new CompressorLogger());

$tests = [
    'Array form' => ['foo', 'bar'],
    'Empty array' => [],
    'Empty string' => '',
    'Comma within element' => ['foo,bar'],
    'String form' => 'foo,bar',
    'Embedded NUL array' => ["foo\0ignored", 'bar'],
    'Embedded NUL string' => "foo\0ignored,bar",
];

foreach ($tests as $name => $compressors) {
    printf("%s:\n", $name);
    new MongoDB\Driver\Manager(null, ['compressors' => $compressors]);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Array form:
Unsupported compressor
Unsupported compressor
Empty array:
Empty string:
Comma within element:
Unsupported compressor
Unsupported compressor
String form:
Unsupported compressor
Unsupported compressor
Embedded NUL array:
Unsupported compressor
Embedded NUL string:
Unsupported compressor
===DONE===
