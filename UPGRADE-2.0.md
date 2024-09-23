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
 * The `MongoDB\Driver\ReadPreference` class now requires a string value for its
   constructor's `$mode` parameter. The integer constants for modes have been
   removed along with the `getMode()` method. Use the string constants and
   `getModeString()` instead.
 * All tentative return types defined in interface and non-final classes are now
   fixed and are required in implementing or extending classes.
 * `MongoDB\Driver\CursorInterface` now extends `Iterator`, requiring 
   implementing classes to also implement iterator methods. The return types for
   the `key` and `current` methods have been narrowed to the types returned by
   cursor instances.
 * The `MongoDB\Driver\CursorId` class was removed. 
   `MongoDB\Driver\Cursor::getId()` and
   `MongoDB\Driver\CursorInterface::getId()` now return a `MongoDB\BSON\Int64`
   instance.
 * The `--with-libbson` and `--with-libmongoc` configure options have been
   removed. Use `--with-mongodb-system-libs` instead.
 * All classes that previously implemented the `Serializable` interface no
   longer implement this interface.
 * The constructor of `MongoDB\BSON\UTCDateTime` no longer accepts a `string`
   argument. To pass 64-bit integers on 32-bit platforms, use the
   `MongoDB\BSON\Int64` class instead.
 * The `--with-openssl-dir` configure option has been removed. If using OpenSSL,
   ensure that it is detected by `pkg-config`.
