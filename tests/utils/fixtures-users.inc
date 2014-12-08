<?php

if ( ! is_readable(__DIR__ . '/../../vendor/autoload.php')) {
    exit('skip composer dependencies not available');
}

require_once __DIR__ . '/basic.inc';
require_once __DIR__ . '/../../vendor/autoload.php';

$faker = Faker\Factory::create();
$faker->seed(1234);

$manager = new MongoDB\Manager(MONGODB_URI);
$batch = new MongoDB\WriteBatch;

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

for ($i = 0; $i < 100; $i++) {
    $batch->insert(createUser($faker));
}

$retval = $manager->executeWriteBatch(NS, $batch);

if ($retval->getInsertedCount() !== 100) {
    exit(sprintf('skip Fixtures were not loaded (expected: %d, actual: %d)', 100, $retval->getInsertedCount()));
}
