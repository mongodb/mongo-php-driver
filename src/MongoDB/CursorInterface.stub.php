<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\Driver;

interface CursorInterface extends \Traversable
{
    /** @tentative-return-type */
    public function getId(): CursorId|\MongoDB\BSON\Int64;

    /** @tentative-return-type */
    public function getServer(): Server;

    /** @tentative-return-type */
    public function isDead(): bool;

    /** @tentative-return-type */
    public function setTypeMap(array $typemap): void;

    /** @tentative-return-type */
    public function toArray(): array;
}
