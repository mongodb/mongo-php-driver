--TEST--
PHPC-923: Use zend_string_release() to free class names (__pclass)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

/* Register an autoloader that does nothing more than append the class name to
 * an array. This adds a reference to the zend_string in PHP 7, which exposes an
 * assert failure if the value is then freed with zend_string_free() instead of
 * zend_string_release(). */
$classes = [];

spl_autoload_register(function ($class) use (&$classes) {
    $classes[] = $class;
});

/* Note: An exception is not thrown if the __pclass field fails to denote a
 * valid class. Instead, it is left as-is and the general type map applies. */
var_dump(toPHP(fromJSON('{"x":{"__pclass":{"$binary":"TWlzc2luZ0NsYXNz","$type":"80"}}}')));

var_dump($classes);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["x"]=>
  object(stdClass)#%d (%d) {
    ["__pclass"]=>
    object(MongoDB\BSON\Binary)#%d (%d) {
      ["data"]=>
      string(12) "MissingClass"
      ["type"]=>
      int(128)
    }
  }
}
array(1) {
  [0]=>
  string(12) "MissingClass"
}
===DONE===
