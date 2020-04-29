--TEST--
MongoDB\Driver\Manager::__construct(): authSource option
--FILE--
<?php

$tests = [
    ['mongodb://username@127.0.0.1/?authSource=$external', []],
    [null, ['authSource' => '$external']],
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
