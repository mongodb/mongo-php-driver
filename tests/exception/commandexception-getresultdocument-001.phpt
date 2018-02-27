--TEST--
MongoDB\Driver\Exception\CommandException::getResultDocument()
--FILE--
<?php

$exception = new MongoDB\Driver\Exception\CommandException();
$resultDocument = ['x' => 1];

$reflection = new ReflectionClass($exception);

$resultDocumentProperty = $reflection->getProperty('resultDocument');
$resultDocumentProperty->setAccessible(true);
$resultDocumentProperty->setValue($exception, $resultDocument);

var_dump($resultDocument === $exception->getResultDocument());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===
