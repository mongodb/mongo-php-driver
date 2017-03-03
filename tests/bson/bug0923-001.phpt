--TEST--
PHPC-923: Use zend_string_release() to free class names (type map)
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

echo throws(function() {
    var_dump(toPHP(fromJSON('{"x":1}'), ['root' => 'MissingClass']));
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

var_dump($classes);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist
array(1) {
  [0]=>
  string(12) "MissingClass"
}
===DONE===
