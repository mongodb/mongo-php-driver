--TEST--
MongoDB\Driver\Manager::__construct(): serverApi driver option (error)
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

echo throws(function () {
    create_test_manager(URI, [], ['serverApi' => '1']);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "serverApi" driver option to be MongoDB\Driver\ServerApi, string given
===DONE===
