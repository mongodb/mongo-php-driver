--TEST--
MongoDB\Driver\Manager::__construct(): "compressors" URI option accepts a string or array of strings
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
    'String form' => 'foo,bar',
    'Array form' => ['foo', 'bar'],
    'Array form with one element' => ['foo'],
    'Empty string' => '',
    'Empty array' => [],
];

foreach ($tests as $name => $compressors) {
    printf("%s:\n", $name);
    new MongoDB\Driver\Manager(null, ['compressors' => $compressors], ['disableClientPersistence' => true]);
}

$byReference = ['foo', 'bar'];
$options = ['compressors' => &$byReference];
new MongoDB\Driver\Manager(null, $options, ['disableClientPersistence' => true]);

printf("Referenced array: %s\n", var_export($byReference, true));
printf("Referenced option: %s\n", var_export($options['compressors'], true));

$byReferenceString = 'foo,bar';
$options = ['compressors' => &$byReferenceString];
new MongoDB\Driver\Manager(null, $options, ['disableClientPersistence' => true]);

printf("Referenced string: %s\n", var_export($byReferenceString, true));
printf("Referenced option: %s\n", var_export($options['compressors'], true));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
String form:
Unsupported compressor
Unsupported compressor
Array form:
Unsupported compressor
Unsupported compressor
Array form with one element:
Unsupported compressor
Empty string:
Empty array:
Unsupported compressor
Unsupported compressor
Referenced array: array (
  0 => 'foo',
  1 => 'bar',
)
Referenced option: array (
  0 => 'foo',
  1 => 'bar',
)
Unsupported compressor
Unsupported compressor
Referenced string: 'foo,bar'
Referenced option: 'foo,bar'
===DONE===
