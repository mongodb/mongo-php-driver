--TEST--
MongoDB\Driver\Manager::__construct(): invalid read preference (mode and tags)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

// Invalid types in URI string

echo throws(function() {
    create_test_manager('mongodb://127.0.0.1/?readPreference=1');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager('mongodb://127.0.0.1/?readPreference=secondary&readPreferenceTags=invalid');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

// Invalid types in URI options array

echo throws(function() {
    create_test_manager(null, ['readPreference' => 1]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager(null, ['readPreference' => 'primary', 'readPreferenceTags' => 'invalid']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

// Invalid values

echo throws(function() {
    create_test_manager('mongodb://127.0.0.1/?readPreference=primary&readPreferenceTags=dc:ny');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager(null, ['readPreference' => 'nothing']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager('mongodb://127.0.0.1/?readPreference=primary', ['readPreferenceTags' => [[]]]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager('mongodb://127.0.0.1/?readPreference=primary', ['readPreferenceTags' => ['invalid']]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://127.0.0.1/?readPreference=1'. Error while assigning URI read preference: Unsupported readPreference value [readPreference=1].
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://127.0.0.1/?readPreference=secondary&readPreferenceTags=invalid'. Unsupported value for "readPreferenceTags": "invalid".
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "readPreference" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected array for "readPreferenceTags" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://127.0.0.1/?readPreference=primary&readPreferenceTags=dc:ny'. Invalid readPreferences.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Unsupported readPreference value: 'nothing'
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Primary read preference mode conflicts with tags
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Read preference tags must be an array of zero or more documents
===DONE===
