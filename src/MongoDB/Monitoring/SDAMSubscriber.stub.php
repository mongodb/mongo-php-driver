<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\Driver\Monitoring;

interface SDAMSubscriber extends Subscriber
{
    public function serverChanged(ServerChangedEvent $event): void;

    public function serverClosed(ServerClosedEvent $event): void;

    public function serverOpening(ServerOpeningEvent $event): void;

    public function serverHeartbeatFailed(ServerHeartbeatFailedEvent $event): void;

    public function serverHeartbeatStarted(ServerHeartbeatStartedEvent $event): void;

    public function serverHeartbeatSucceeded(ServerHeartbeatSucceededEvent $event): void;

    public function topologyChanged(TopologyChangedEvent $event): void;

    public function topologyClosed(TopologyClosedEvent $event): void;

    public function topologyOpening(TopologyOpeningEvent $event): void;
}
