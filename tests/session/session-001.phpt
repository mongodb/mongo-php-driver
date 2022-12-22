--TEST--
MongoDB\Driver\Session spec test: Pool is LIFO
--DESCRIPTION--
Session spec prose test #2: Pool is LIFO
https://github.com/mongodb/specifications/blob/master/source/sessions/tests/README.rst#pool-is-lifo
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$firstSession = $manager->startSession();
$firstSessionId = $firstSession->getLogicalSessionId();

/* libmongoc does not pool unused sessions (CDRIVER-3322), so we must use this
 * session with a command to ensure it enters the pool. */
$command = new MongoDB\Driver\Command(['ping' => 1]);
$manager->executeCommand(DATABASE_NAME, $command, ['session' => $firstSession]);

unset($firstSession);

$secondSession = $manager->startSession();
$secondSessionId = $secondSession->getLogicalSessionId();

var_dump($firstSessionId == $secondSessionId);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===
