--TEST--
ini_get() reports mongodb.debug (master and local)
--INI--
mongodb.debug=stderr
--FILE--
<?php

/* Note: on Linux, trace logs for _mongoc_linux_distro_scanner_get_distro are
 * emitted when mongoc_init is called. We echo an additional line here to avoid
 * having "%A" consume the first var_dump() output on Windows. */
echo "mongodb.debug\n";

var_dump(ini_get('mongodb.debug'));
ini_set('mongodb.debug', 'stdout');
var_dump(ini_get('mongodb.debug'));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%A
string(6) "stderr"
string(6) "stdout"
===DONE===
