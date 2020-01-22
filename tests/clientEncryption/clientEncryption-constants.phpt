--TEST--
MongoDB\Driver\ClientEncryption constants
--FILE--
<?php

var_dump(MongoDB\Driver\ClientEncryption::AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC);
var_dump(MongoDB\Driver\ClientEncryption::AEAD_AES_256_CBC_HMAC_SHA_512_RANDOM);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(43) "AEAD_AES_256_CBC_HMAC_SHA_512-Deterministic"
string(36) "AEAD_AES_256_CBC_HMAC_SHA_512-Random"
===DONE===
