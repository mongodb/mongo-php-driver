<?php

/**
 * @generate-class-entries
 * @generate-legacy-arginfo 80100
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
