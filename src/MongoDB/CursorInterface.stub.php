<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\Driver;

interface CursorInterface extends \Iterator
{
    public function getId(): CursorId|\MongoDB\BSON\Int64;

    public function getServer(): Server;

    public function isDead(): bool;

    public function setTypeMap(array $typemap): void;

    public function toArray(): array;
}
