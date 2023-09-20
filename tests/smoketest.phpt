--TEST--
Smoke test to make sure compilation succeeded
--FILE--
<?php
var_dump(extension_loaded('mongodb'));
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===
