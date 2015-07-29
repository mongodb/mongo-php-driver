--TEST--
BSON\toPHP(): Type map class must be both instantiatable and Unserializable
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
use MongoDB\BSON as BSON;

require_once __DIR__ . "/../utils/basic.inc";

abstract class MyAbstractDocument implements BSON\Unserializable
{
}

class MyDocument
{
}

$bson = fromJSON('{ "list" : [ 1, 2, 3 ], "map" : { "foo" : "bar" } }');
$types = array('array', 'document', 'root');
$classNames = array(
    'MissingClass',
    'MyAbstractDocument',
    'MyDocument',
    BSON_NAMESPACE . '\Unserializable'
);

foreach ($types as $type) {
    foreach ($classNames as $className) {
        printf("Test typeMap: { \"%s\" : \"%s\" }\n", $type, $className);
        try {
            var_dump(toPHP($bson, array($type => $className)));
        } catch (MongoDB\Driver\Exception\InvalidArgumentException $e) {
            echo $e->getMessage(), "\n";
        }
        echo "\n";
    }
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Test typeMap: { "array" : "MissingClass" }
Class MissingClass does not exist

Test typeMap: { "array" : "MyAbstractDocument" }
Class MyAbstractDocument is not instantiatable

Test typeMap: { "array" : "MyDocument" }
Class MyDocument does not implement %SBSON\Unserializable

Test typeMap: { "array" : "%SBSON\Unserializable" }
Class %SBSON\Unserializable is not instantiatable

Test typeMap: { "document" : "MissingClass" }
Class MissingClass does not exist

Test typeMap: { "document" : "MyAbstractDocument" }
Class MyAbstractDocument is not instantiatable

Test typeMap: { "document" : "MyDocument" }
Class MyDocument does not implement %SBSON\Unserializable

Test typeMap: { "document" : "%SBSON\Unserializable" }
Class %SBSON\Unserializable is not instantiatable

Test typeMap: { "root" : "MissingClass" }
Class MissingClass does not exist

Test typeMap: { "root" : "MyAbstractDocument" }
Class MyAbstractDocument is not instantiatable

Test typeMap: { "root" : "MyDocument" }
Class MyDocument does not implement %SBSON\Unserializable

Test typeMap: { "root" : "%SBSON\Unserializable" }
Class %SBSON\Unserializable is not instantiatable

===DONE===
