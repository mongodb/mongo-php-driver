--TEST--
MongoDB\Driver\Manager::executeQuery() exposes error document via CommandException
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$query = new MongoDB\Driver\Query(['field' => ['$unsupportedOperator' => true]]);

try {
    $manager->executeQuery(NS, $query);
} catch (\MongoDB\Driver\Exception\CommandException $e) {
    printf("%s(%d): %s\n", get_class($e), $e->getCode(), $e->getMessage());
    $doc = $e->getResultDocument();
    var_dump($doc->errmsg === $e->getMessage());
    var_dump($doc->code === $e->getCode());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
MongoDB\Driver\Exception\CommandException(2): unknown operator: $unsupportedOperator
bool(true)
bool(true)
===DONE===
