--TEST--
MongoDB\Driver\Manager: Writing debug log files
--FILE--
<?php

$name = tempnam(sys_get_temp_dir(), "PHONGO");
unlink($name);
mkdir($name);

ini_set('mongodb.debug', $name);
$manager = new MongoDB\Driver\Manager;
ini_set('mongodb.debug', 'off');

foreach (glob($name."/*") as $file) {
    echo file_get_contents($file);
    unlink($file);
}
rmdir($name);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%A[%s]     PHONGO: DEBUG   > Connection string: '%s'
[%s]     PHONGO: DEBUG   > Creating Manager, phongo-1.%d.%d%S[%s] - mongoc-1.%s(%s), libbson-1.%s(%s), php-%s
%A===DONE===%A
