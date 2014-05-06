<?php
namespace MongoDB;

class Server {
    const TYPE_MONGOS     = 0x01;
    const TYPE_STANDALONE = 0x02;
    const TYPE_ARBITER    = 0x03;
    const TYPE_SECONDARY  = 0x04;
    const TYPE_PRIMARY    = 0x05;

    /* These need to be final as they will not be called by the driver to retrieve the 
     * information, these getters are only useful for userland */
    final function getHostname() {}
    final function getPort() {}
    final function getLatency() {}
    final function getMaxWireVersion() {}
    final function getMinWireVersion() {}
    final function getServerType() {}

    /* extracting config info */
    final function isPassive() {}
    final function isDelayed() {}

    final function executeQuery($namespace, \MongoDB\Query $query) {}
    final function executeWrite($namespace, \MongoDB\WriteBatch $batch, WriteOptions $wo) {}
}


class Manager extends \SplObjectStorage {
    function __construct(array $servers, $dbname, $connection_options) {
    }

    static function fromDSN($dsn) {
    }

    final function executeQuery($namespace, \MongoDB\Query $query, ReadPreference $rp) {
        foreach($this->getConnectedServers() as $server) {
            if ($server->matchesReadPreference($rp)) {
                return $server->executeQuery($namespace, $query);
            }
        }
        throw new NoServerMatchingReadPreference($rp);
    }

    final function executeWrite($namespace, \MongoDB\WriteBatch $batch, WriteOptions $wo) {
        foreach($this->getConnectedServers() as $server) {
            if ($server->isPrimary()) {
                return $server->executeWrite($namespace, $batch, $wo);
            }
        }

        throw new NoPrimaryAvailable;
    }
}

