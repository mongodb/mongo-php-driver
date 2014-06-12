<?php

namespace MongoDB\Write;

interface UpdateResult extends WriteResult
{
    /**
     * @return integer
     */
    function getNumMatched();

    /**
     * @return integer
     */
    function getNumModified();

    /**
     * @return integer
     */
    function getNumUpserted();

    /**
     * @return Upsert[]
     */
    function getUpserts();
}
