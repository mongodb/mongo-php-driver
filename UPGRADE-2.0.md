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
