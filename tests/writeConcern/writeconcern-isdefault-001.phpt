--TEST--
MongoDB\Driver\WriteConcern::isDefault()
--FILE--
<?php

$tests = [
    new MongoDB\Driver\WriteConcern(-3), // MONGOC_WRITE_CONCERN_W_MAJORITY
    new MongoDB\Driver\WriteConcern(-2), // MONGOC_WRITE_CONCERN_W_DEFAULT
    new MongoDB\Driver\WriteConcern(-1), // MONGOC_WRITE_CONCERN_W_ERRORS_IGNORED
    new MongoDB\Driver\WriteConcern(0),  // MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED
    new MongoDB\Driver\WriteConcern(1),
    new MongoDB\Driver\WriteConcern(2),
    new MongoDB\Driver\WriteConcern('tag'),
    new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY),
    // mongoc_uri_parse_option() ignores empty string for w
    (new MongoDB\Driver\Manager('mongodb://127.0.0.1/?w='))->getWriteConcern(),
    // Cannot test "w=-3" since libmongoc URI parsing expects integers >= -1
    // Cannot test "w=-2" since libmongoc URI parsing expects integers >= -1, and throws an error otherwise
    (new MongoDB\Driver\Manager('mongodb://127.0.0.1/?w=-1'))->getWriteConcern(),
    (new MongoDB\Driver\Manager('mongodb://127.0.0.1/?w=0'))->getWriteConcern(),
    (new MongoDB\Driver\Manager('mongodb://127.0.0.1/?w=1'))->getWriteConcern(),
    (new MongoDB\Driver\Manager('mongodb://127.0.0.1/?w=2'))->getWriteConcern(),
    (new MongoDB\Driver\Manager('mongodb://127.0.0.1/?w=tag'))->getWriteConcern(),
    (new MongoDB\Driver\Manager('mongodb://127.0.0.1/?w=majority'))->getWriteConcern(),
    // Cannot test ['w' => null] since an integer or string type is expected (PHPC-887)
    // Cannot test ['w' => -3] or ['w' => -2] since php_phongo_apply_wc_options_to_uri() expects integers >= -1
    (new MongoDB\Driver\Manager(null, ['w' => -1]))->getWriteConcern(),
    (new MongoDB\Driver\Manager(null, ['w' => 0]))->getWriteConcern(),
    (new MongoDB\Driver\Manager(null, ['w' => 1]))->getWriteConcern(),
    (new MongoDB\Driver\Manager(null, ['w' => 2]))->getWriteConcern(),
    (new MongoDB\Driver\Manager(null, ['w' => 'tag']))->getWriteConcern(),
    (new MongoDB\Driver\Manager(null, ['w' => 'majority']))->getWriteConcern(),
    (new MongoDB\Driver\Manager)->getWriteConcern(),
];

foreach ($tests as $wc) {
    var_dump($wc->isDefault());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(false)
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(true)
===DONE===
