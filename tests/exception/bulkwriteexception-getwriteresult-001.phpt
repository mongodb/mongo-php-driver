--TEST--
MongoDB\Driver\Exception\BulkWriteException::getWriteResult()
--FILE--
<?php

$exception = new MongoDB\Driver\Exception\BulkWriteException();
$writeResult = ['x' => 1];

$reflection = new ReflectionClass($exception);

$resultDocumentProperty = $reflection->getProperty('writeResult');
$resultDocumentProperty->setAccessible(true);
$resultDocumentProperty->setValue($exception, $writeResult);

var_dump($writeResult === $exception->getWriteResult());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===
