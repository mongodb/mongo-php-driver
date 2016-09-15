--TEST--
MongoDB\Driver\ReadPreference cannot be extended
--FILE--
<?php

class MyReadPreference extends MongoDB\Driver\ReadPreference {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyReadPreference may not inherit from final class (MongoDB\Driver\ReadPreference) in %s on line %d
