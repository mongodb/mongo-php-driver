<?php

namespace MongoDB;

/**
 * Value object for read preferences used in issuing commands and queries.
 */
final class ReadPreference
{
    const RP_PRIMARY             = 'primary';
    const RP_PRIMARY_PREFERRED   = 'primaryPreferred';
    const RP_SECONDARY           = 'secondary';
    const RP_SECONDARY_PREFERRED = 'secondaryPreferred';
    const RP_NEAREST             = 'nearest';

    private $readPreference;
    private $tagSets;

    /**
     * @param string  $readPreference Read preference name
     * @param array[] $tagSets        Array of zero or more tag sets (i.e. tag criteria)
     */
    public function __construct($readPreference, array $tagSets = null)
    {
        $this->readPreference = (string) $readPreference;
        $this->tagSets = $tagSets;
    }
}
