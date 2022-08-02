<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\Driver\Monitoring;

interface SDAMSubscriber extends Subscriber
{
    /** @tentative-return-type */
    public function serverChanged(ServerChangedEvent $event): void;

    /** @tentative-return-type */
    public function serverClosed(ServerClosedEvent $event): void;

    /** @tentative-return-type */
    public function serverOpening(ServerOpeningEvent $event): void;

    /** @tentative-return-type */
    public function serverHeartbeatFailed(ServerHeartbeatFailedEvent $event): void;

    /** @tentative-return-type */
    public function serverHeartbeatStarted(ServerHeartbeatStartedEvent $event): void;

    /** @tentative-return-type */
    public function serverHeartbeatSucceeded(ServerHeartbeatSucceededEvent $event): void;

    /** @tentative-return-type */
    public function topologyChanged(TopologyChangedEvent $event): void;

    /** @tentative-return-type */
    public function topologyClosed(TopologyClosedEvent $event): void;

    /** @tentative-return-type */
    public function topologyOpening(TopologyOpeningEvent $event): void;
}
