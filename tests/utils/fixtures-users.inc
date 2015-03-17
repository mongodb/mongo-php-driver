<?php

if ( ! is_readable(__DIR__ . '/../../vendor/autoload.php')) {
    exit('skip composer dependencies not available');
}

$LIMIT = isset($LIMIT) ? (int)$LIMIT : 1024;

require_once __DIR__ . '/basic.inc';
require_once __DIR__ . '/../../vendor/autoload.php';

function createUser($faker)
{
    return array(
        'username' => $faker->unique()->userName,
        'password' => $faker->sha256,
        'email' => $faker->unique()->safeEmail,
        'firstName' => $faker->firstName,
        'lastName' => $faker->lastName,
        'phoneNumber' => $faker->phoneNumber,
        'altPhoneNumber' => $faker->optional(0.1)->phoneNumber,
        'company' => $faker->company,
        'bio' => $faker->paragraph,
        'createdAt' => $faker->dateTimeBetween('2008-01-01T00:00:00+0000', '2014-08-01T00:00:00+0000')->getTimestamp(),
        'addresses' => array(
            createAddress($faker),
            createAddress($faker),
            createAddress($faker),
        ),
    );
}

function createAddress($faker)
{
    return array(
        'streetAddress' => $faker->streetAddress,
        'city' => $faker->city,
        'state' => $faker->state,
        'postalCode' => $faker->postcode,
        'loc' => createGeoJsonPoint($faker),
    );
}

function createGeoJsonPoint($faker)
{
    return array(
        'type' => 'Point',
        'coordinates' => array($faker->longitude, $faker->latitude),
    );
}

$CACHE =  sys_get_temp_dir() . "/PHONGO-FIXTURES.json";

$manager = new MongoDB\Driver\Manager(STANDALONE);
$bulk = new MongoDB\Driver\BulkWrite;


if (file_exists($CACHE) && filemtime($CACHE) > filemtime(__FILE__)) {
    $data = file_get_contents($CACHE);
    $users = json_decode($data, true);
} else {
    $faker = Faker\Factory::create();
    $faker->seed(1234);

    $users = array();
    for ($i = 0; $i < 1024; $i++) {
        $users[] = $user = createUser($faker);
    }

    file_put_contents($CACHE, json_encode($users));
}

foreach($users as $n => $user) {
    $bulk->insert($user);

    if ($n >= $LIMIT-1) {
        break;
    }
}
$retval = $manager->executeBulkWrite(NS, $bulk);

if ($retval->getInsertedCount() !== $LIMIT) {
    exit(sprintf('skip Fixtures were not loaded (expected: %d, actual: %d)', $LIMIT, $retval->getInsertedCount()));
}


$cmd = array(
    "createIndexes" => COLLECTION_NAME,
    "indexes" => array(
        array(
            "key" => array("_id" => 1, "username" => 1),
            "name" => "cursorid_test",
            "unique" => true,
        ),
    ),
);
$index = new MongoDB\Driver\Command($cmd);
$retval = $manager->executeCommand(DATABASE_NAME, $index);

