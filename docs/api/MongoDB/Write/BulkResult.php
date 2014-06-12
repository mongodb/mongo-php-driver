<?php

namespace MongoDB\Write;

interface BulkResult extends InsertResult, UpdateResult, DeleteResult
{
    // Note: this interface would exist alongside Bulk API in the userland lib
}
