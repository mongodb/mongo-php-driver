UPGRADE FROM 1.x to 2.0
=======================

 * The `getServer()` method has been removed from the CommandFailedEvent,
   CommandStartedEvent, and CommandSucceededEvent event classes. The `getHost()`
   and `getPort()` methods have been added in its place.
