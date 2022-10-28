--TEST--
Enums implementing Serializable round-tripped through parent document
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.1.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* Although non-backed enums cannot be serialized directly, they can implement
 * Serializable like any other object. This allows a non-backed enum to be
 * round-tripped in conjunction with a parent document's bsonUnserialize()
 * method. */
enum MyEnum implements MongoDB\BSON\Serializable
{
    case A;

    public function bsonSerialize(): array
    {
        return (array) $this;
    }
}

enum MyBackedEnum: string implements MongoDB\BSON\Serializable
{
    case A = 'a';

    public function bsonSerialize(): array
    {
        return (array) $this;
    }
}

class MyDocument implements MongoDB\BSON\Persistable
{
    public function __construct(
        private MyEnum $x,
        private MyBackedEnum $y
    ) {}

    public function bsonSerialize(): array
    {
        return [
            'x' => $this->x,
            'y' => $this->y,
        ];
    }

    public function bsonUnserialize(array $data): void
    {
        if (isset($data['x'])) {
            // See: https://www.php.net/manual/en/language.enumerations.basics.php#127112
            $this->x = constant(MyEnum::class . '::' . $data['x']->name);
        }

        if (isset($data['y'])) {
            $this->y = MyBackedEnum::from($data['y']->value);
        }
    }
}

$document = new MyDocument(MyEnum::A, MyBackedEnum::A);

var_dump(toPHP(fromPHP($document)));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MyDocument)#%d (%d) {
  ["x":"MyDocument":private]=>
  enum(MyEnum::A)
  ["y":"MyDocument":private]=>
  enum(MyBackedEnum::A)
}
===DONE===
