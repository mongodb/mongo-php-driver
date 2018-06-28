--TEST--
MongoDB\Driver\Exception\RuntimeException::hasErrorLabel() with non-array values
--FILE--
<?php

$exception = new MongoDB\Driver\Exception\RuntimeException();
$labels = 'shouldBeAnArray';

$reflection = new ReflectionClass($exception);

$resultDocumentProperty = $reflection->getProperty('errorLabels');
$resultDocumentProperty->setAccessible(true);
$resultDocumentProperty->setValue($exception, $labels);

var_dump($exception->hasErrorLabel('bar'));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(false)
===DONE===
