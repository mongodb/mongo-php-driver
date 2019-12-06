--TEST--
MongoDB\Driver\Session constants
--FILE--
<?php

var_dump(MongoDB\Driver\Session::TRANSACTION_NONE);
var_dump(MongoDB\Driver\Session::TRANSACTION_STARTING);
var_dump(MongoDB\Driver\Session::TRANSACTION_IN_PROGRESS);
var_dump(MongoDB\Driver\Session::TRANSACTION_COMMITTED);
var_dump(MongoDB\Driver\Session::TRANSACTION_ABORTED);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
string(4) "none"
string(8) "starting"
string(11) "in_progress"
string(9) "committed"
string(7) "aborted"
===DONE===
