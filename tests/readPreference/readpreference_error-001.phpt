--TEST--
MongoDB\Driver\ReadPreference cannot be extended
--FILE--
<?php

class MyReadPreference extends MongoDB\Driver\ReadPreference {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyReadPreference %s final class %SMongoDB\Driver\ReadPreference%S in %s on line %d
