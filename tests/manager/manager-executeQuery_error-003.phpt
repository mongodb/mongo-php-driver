--TEST--
MongoDB\Driver\Manager::executeQuery() exposes error document via CommandException
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '3.2'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
$query = new MongoDB\Driver\Query(['$foo' => 1]);

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
MongoDB\Driver\Exception\CommandException(2): unknown top level operator: $foo
bool(true)
bool(true)
===DONE===
