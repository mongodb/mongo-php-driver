--TEST--
MongoDB\Driver\Manager::__construct(): authMechanism option
--FILE--
<?php

$tests = [
    ['mongodb://username@127.0.0.1/?authMechanism=MONGODB-X509', []],
    ['mongodb://127.0.0.1/?authMechanism=MONGODB-X509', []],
    ['mongodb://username@127.0.0.1/?authMechanism=GSSAPI', []],
    [null, ['authMechanism' => 'MONGODB-X509', 'username' => 'username']],
    [null, ['authMechanism' => 'MONGODB-X509']],
    [null, ['authMechanism' => 'GSSAPI', 'username' => 'username']],
];

foreach ($tests as $test) {
    list($uri, $options) = $test;

    /* Note: the Manager's debug information does not include the auth mechanism
     * so we are merely testing that no exception is thrown. */
    $manager = new MongoDB\Driver\Manager($uri, $options);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===DONE===
