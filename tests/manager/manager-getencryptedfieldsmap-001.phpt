--TEST--
MongoDB\Driver\Manager::getEncryptedFieldsMap()
--FILE--
<?php

/* autoEncryption requires keyVaultNamespace and a kmsProvider. Additionally,
 * disable mongocryptd spawning since it is not required for this test. */
$baseOptions = [
    'keyVaultNamespace' => 'encryption.__keyVault',
    'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary(str_repeat('0', 96), 0)]],
    'extraOptions' => ['mongocryptdBypassSpawn' => true],
];

$encryptedFields = [
    'escCollection' => 'escCollectionName',
    'eccCollection' => 'eccCollectionName',
    'ecocCollection' => 'ecocCollectionName',
    'fields' => [
        [
            'path' => 'foo',
            'keyId' => new MongoDB\BSON\Binary(str_repeat('0', 16), MongoDB\BSON\Binary::TYPE_UUID),
            'bsonType' => 'string',
            'queries' => ['queryType' => 'equality'],
        ],
    ],
];

$tests = [
    [],
    ['autoEncryption' => $baseOptions],
    ['autoEncryption' => ['encryptedFieldsMap' => []] + $baseOptions],
    ['autoEncryption' => ['encryptedFieldsMap' => ['db.coll' => $encryptedFields]] + $baseOptions],
];

foreach ($tests as $i => $driverOptions) {
    $manager = new MongoDB\Driver\Manager(null, [], $driverOptions);
    var_dump($manager->getEncryptedFieldsMap());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
NULL
NULL
array(0) {
}
array(1) {
  ["db.coll"]=>
  array(4) {
    ["escCollection"]=>
    string(17) "escCollectionName"
    ["eccCollection"]=>
    string(17) "eccCollectionName"
    ["ecocCollection"]=>
    string(18) "ecocCollectionName"
    ["fields"]=>
    array(1) {
      [0]=>
      array(4) {
        ["path"]=>
        string(3) "foo"
        ["keyId"]=>
        object(MongoDB\BSON\Binary)#%d (%d) {
          ["data"]=>
          string(16) "0000000000000000"
          ["type"]=>
          int(4)
        }
        ["bsonType"]=>
        string(6) "string"
        ["queries"]=>
        array(1) {
          ["queryType"]=>
          string(8) "equality"
        }
      }
    }
  }
}
===DONE===
