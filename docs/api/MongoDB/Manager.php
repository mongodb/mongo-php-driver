<?php

namespace MongoDB;

final class Manager extends \SplObjectStorage
{
    /**
     * @param Server[] $servers
     * @param string   $db      Database name
     * @param array    $options Connection options
     */
    public function __construct(array $servers, $db, $options)
    {
    }

    static public function fromDSN($dsn)
    {
    }

    final public function executeCommand($namespace, \MongoDB\Query $query, \MongoDB\ReadPreference $rp)
    {
        foreach($this->getConnectedServers() as $server) {
            if ($server->matchesReadPreference($rp)) {
                return $server->executeQuery($namespace, $query);
            }
        }
        throw new NoServerMatchingReadPreference($rp);
    }

    final public function executeQuery($namespace, \MongoDB\Query $query, \MongoDB\ReadPreference $rp)
    {
        foreach($this->getConnectedServers() as $server) {
            if ($server->matchesReadPreference($rp)) {
                return $server->executeQuery($namespace, $query);
            }
        }
        throw new NoServerMatchingReadPreference($rp);
    }

    final function executeWrite($namespace, \MongoDB\WriteBatch $batch, \MongoDB\WriteOptions $wo)
    {
        foreach($this->getConnectedServers() as $server) {
            if ($server->isPrimary()) {
                return $server->executeWrite($namespace, $batch, $wo);
            }
        }

        throw new NoPrimaryAvailable;
    }
}
