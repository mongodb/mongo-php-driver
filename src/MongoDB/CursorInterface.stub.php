<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\Driver;

interface CursorInterface extends \Iterator
{
    public function current(): array|object|null {}

    public function getId(): \MongoDB\BSON\Int64;

    public function getServer(): Server;

    public function isDead(): bool;

    public function key(): ?int;

    public function setTypeMap(array $typemap): void;

    public function toArray(): array;
}
