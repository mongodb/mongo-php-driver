<?php

namespace MongoDB;

class Server
{
    const TYPE_MONGOS     = 0x01;
    const TYPE_STANDALONE = 0x02;
    const TYPE_ARBITER    = 0x03;
    const TYPE_SECONDARY  = 0x04;
    const TYPE_PRIMARY    = 0x05;

    /* These need to be final as they will not be called by the driver to
     * retrieve the information; these getters are only useful for userland.
     */
    final public function getHostname() {}
    final public function getPort() {}
    final public function getLatency() {}
    final public function getMaxWireVersion() {}
    final public function getMinWireVersion() {}
    final public function getServerType() {}

    // extracting config info
    final public function isPassive() {}
    final public function isDelayed() {}

    final public function executeCommand($namespace, \MongoDB\Command\Command $command, \MongoDB\ReadPreference $rp) {}
    final public function executeQuery($namespace, \MongoDB\Query\Query $query, \MongoDB\ReadPreference $rp) {}
    final public function executeWrite($namespace, \MongoDB\Write\WriteBatch $batch, \MongoDB\WriteOptions $wo) {}
}
