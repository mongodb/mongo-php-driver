--TEST--
MongoDB\Driver\Manager::__construct(): authMechanismProperties option
--FILE--
<?php

$tests = [
    ['mongodb://username@127.0.0.1/?authMechanism=GSSAPI&authMechanismProperties=CANONICALIZE_HOST_NAME:true,SERVICE_NAME:foo,SERVICE_REALM:bar', []],
    [null, ['username' => 'username', 'authMechanism' => 'GSSAPI', 'authMechanismProperties' => ['CANONICALIZE_HOST_NAME' => 'true', 'SERVICE_NAME' => 'foo', 'SERVICE_REALM' => 'bar']]],
    // Options are case-insensitive
    ['mongodb://username@127.0.0.1/?authMechanism=GSSAPI&authMechanismProperties=canonicalize_host_name:TRUE,service_name:foo,service_realm:bar', []],
    [null, ['username' => 'username', 'authMechanism' => 'GSSAPI', 'authMechanismProperties' => ['canonicalize_host_name' => 'TRUE', 'service_name' => 'foo', 'service_realm' => 'bar']]],
    // Boolean true "CANONICALIZE_HOST_NAME" value is converted to "true"
    [null, ['username' => 'username', 'authMechanism' => 'GSSAPI', 'authMechanismProperties' => ['canonicalize_host_name' => true]]],
];

foreach ($tests as $test) {
    list($uri, $options) = $test;

    /* Note: the Manager's debug information does not include the auth mechanism
     * so we are merely testing that no exception is thrown and that option
     * processing does not leak memory. */
    $manager = new MongoDB\Driver\Manager($uri, $options);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===DONE===
