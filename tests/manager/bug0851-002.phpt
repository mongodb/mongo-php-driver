--TEST--
PHPC-851: Manager constructor should not modify driverOptions argument
--FILE--
<?php

$driverOptions = [
    'weak_cert_validation' => true,
    'context' => stream_context_create([
        'ssl' => [
            'allow_self_signed' => true,
        ],
    ]),
];

$manager = new MongoDB\Driver\Manager(null, [], $driverOptions);
var_dump($driverOptions);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(2) {
  ["weak_cert_validation"]=>
  bool(true)
  ["context"]=>
  resource(4) of type (stream-context)
}
===DONE===
