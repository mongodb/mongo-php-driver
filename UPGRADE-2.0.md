UPGRADE FROM 1.x to 2.0
=======================

 * The `getServer()` method has been removed from the CommandFailedEvent,
   CommandStartedEvent, and CommandSucceededEvent event classes. The `getHost()`
   and `getPort()` methods have been added in its place.
 * The BSON functions in the `MongoDB\BSON` namespace have been removed in favor 
   of the `MongoDB\BSON\Document` class.
 * The constants `MongoDB\Driver\ClientEncryption::ALGORITHM_RANGE_PREVIEW` and
   `MongoDB\Driver\ClientEncryption::QUERY_TYPE_RANGE_PREVIEW` have been 
   removed. Use the `ALGORITHM_RANGE` and `QUERY_TYPE_RANGE` instead.
 * All tentative return types defined in interface and non-final classes are now
   fixed and are required in implementing or extending classes.
 * `MongoDB\Driver\CursorInterface` now extends `Iterator`, requiring 
   implementing classes to also implement iterator methods. The return types for
   the `key` and `current` methods have been narrowed to the types returned by
   cursor instances.
