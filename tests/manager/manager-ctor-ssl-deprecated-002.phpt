--TEST--
MongoDB\Driver\Manager::__construct(): Test deprecated options (capath)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_ssl(['OpenSSL', 'LibreSSL']); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

echo raises(
    function () {
        new MongoDB\Driver\Manager('mongodb://127.0.0.1/', [], ['capath' => 'foo']);
    },
    E_DEPRECATED
), "\n";

echo raises(
    function () {
        new MongoDB\Driver\Manager('mongodb://127.0.0.1/', [], ['context' => stream_context_create(['ssl' => ['capath' => 'foo']])]);
    },
    E_DEPRECATED
), "\n";

?>
===DONE===
--EXPECT--
OK: Got E_DEPRECATED
MongoDB\Driver\Manager::__construct(): The "capath" context driver option is deprecated. Please use the "ca_dir" driver option instead.
OK: Got E_DEPRECATED
MongoDB\Driver\Manager::__construct(): The "context" driver option is deprecated.
===DONE===
