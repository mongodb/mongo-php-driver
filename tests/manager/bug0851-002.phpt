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
--EXPECTF--
Deprecated: MongoDB\Driver\Manager::__construct(): The "context" driver option is deprecated.%s

Deprecated: MongoDB\Driver\Manager::__construct(): The "weak_cert_validation" driver option is deprecated. Please use the "tlsAllowInvalidCertificates" URI option instead.%s
array(2) {
  ["weak_cert_validation"]=>
  bool(true)
  ["context"]=>
  resource(4) of type (stream-context)
}
===DONE===
