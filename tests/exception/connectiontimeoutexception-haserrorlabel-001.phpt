--TEST--
MongoDB\Driver\Exception\ConnectionTimeoutException::hasErrorLabel()
--FILE--
<?php

$exception = new MongoDB\Driver\Exception\ConnectionTimeoutException();
$labels = ['test', 'foo'];

$reflection = new ReflectionClass($exception);
$errorLabelsProperty = $reflection->getProperty('errorLabels');
$errorLabelsProperty->setValue($exception, $labels);

var_dump($exception->hasErrorLabel('foo'));
var_dump($exception->hasErrorLabel('bar'));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(false)
===DONE===
