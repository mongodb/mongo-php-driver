--TEST--
MongoDB\BSON\toPHP(): Type map class must be both instantiatable and Unserializable
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

abstract class MyAbstractDocument implements MongoDB\BSON\Unserializable
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
    'MongoDB\BSON\Unserializable'
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
Class MyDocument does not implement MongoDB\BSON\Unserializable

Test typeMap: { "array" : "MongoDB\BSON\Unserializable" }
Class MongoDB\BSON\Unserializable is not instantiatable

Test typeMap: { "document" : "MissingClass" }
Class MissingClass does not exist

Test typeMap: { "document" : "MyAbstractDocument" }
Class MyAbstractDocument is not instantiatable

Test typeMap: { "document" : "MyDocument" }
Class MyDocument does not implement MongoDB\BSON\Unserializable

Test typeMap: { "document" : "MongoDB\BSON\Unserializable" }
Class MongoDB\BSON\Unserializable is not instantiatable

Test typeMap: { "root" : "MissingClass" }
Class MissingClass does not exist

Test typeMap: { "root" : "MyAbstractDocument" }
Class MyAbstractDocument is not instantiatable

Test typeMap: { "root" : "MyDocument" }
Class MyDocument does not implement MongoDB\BSON\Unserializable

Test typeMap: { "root" : "MongoDB\BSON\Unserializable" }
Class MongoDB\BSON\Unserializable is not instantiatable

===DONE===
