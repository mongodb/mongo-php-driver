<?php

require_once __DIR__ . "/" . "tools.php";

define('URI', getenv('MONGODB_URI') ?: 'mongodb://127.0.0.1/');
define('MONGO_ORCHESTRATION_URI', getenv('MONGO_ORCHESTRATION_URI') ?: 'http://localhost:8889/v1');
define('DATABASE_NAME', getenv('MONGODB_DATABASE') ?: 'phongo');
define('COLLECTION_NAME', makeCollectionNameFromFilename($_SERVER['SCRIPT_FILENAME']));
define('NS', DATABASE_NAME . '.' . COLLECTION_NAME);
define('SSL_DIR', realpath(getenv('SSL_DIR')));

if (getenv('MOCK_SERVICE_ID')) {
    ini_set('mongodb.mock_service_id', '1');
}
