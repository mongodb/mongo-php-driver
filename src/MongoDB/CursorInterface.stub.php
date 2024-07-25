<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\Driver;

interface CursorInterface extends \Traversable
{
#if PHP_VERSION_ID >= 80000
    /** @tentative-return-type */
    public function getId(): CursorId|\MongoDB\BSON\Int64;
#else
    /** @return CursorId|\MongoDB\BSON\Int64 */
    public function getId();
#endif

    /** @tentative-return-type */
    public function getServer(): Server;

    /** @tentative-return-type */
    public function isDead(): bool;

    /** @tentative-return-type */
    public function setTypeMap(array $typemap): void;

    /** @tentative-return-type */
    public function toArray(): array;
}
