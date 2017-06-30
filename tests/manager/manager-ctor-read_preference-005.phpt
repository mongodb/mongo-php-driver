--TEST--
MongoDB\Driver\Manager::__construct(): read preference options of the wrong type will throw an InvalidArgumentException exception
--FILE--
<?php

$options = array(
    'readPreference' => []
);

try {
    $manager = new MongoDB\Driver\Manager(null, $options);
} catch (\MongoDB\Driver\Exception\InvalidArgumentException $exception) {
    echo $exception->getMessage() . "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
"readPreference" URI option expected to be string
===DONE===
