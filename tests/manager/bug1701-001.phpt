--TEST--
PHPC-1701: prep_authmechanismproperties may leak if Manager ctor errors
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

echo throws(function () {
    // Using a stream context without SSL options causes an exception in the constructor, triggering the potential leak
    create_test_manager(
            null,
            ['username' => 'username', 'authMechanism' => 'GSSAPI', 'authMechanismProperties' => ['canonicalize_host_name' => true]],
            ['context' => stream_context_create([])]
        );
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Stream-Context resource does not contain "ssl" options array
===DONE===
