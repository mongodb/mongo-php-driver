--TEST--
Backed enums round-tripped through parent document
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.1.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

enum MyIntBackedEnum: int
{
    case A = 1;
}

enum MyStringBackedEnum: string
{
    case A = 'a';
}

class MyDocument implements MongoDB\BSON\Persistable
{
    public function __construct(
        private MyIntBackedEnum $x,
        private MyStringBackedEnum $y
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
            $this->x = MyIntBackedEnum::from($data['x']);
        }

        if (isset($data['y'])) {
            $this->y = MyStringBackedEnum::from($data['y']);
        }
    }
}

$document = new MyDocument(MyIntBackedEnum::A, MyStringBackedEnum::A);

var_dump(toPHP(fromPHP($document)));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MyDocument)#%d (%d) {
  ["x":"MyDocument":private]=>
  enum(MyIntBackedEnum::A)
  ["y":"MyDocument":private]=>
  enum(MyStringBackedEnum::A)
}
===DONE===
