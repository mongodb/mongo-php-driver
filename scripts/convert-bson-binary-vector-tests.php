<?php

use MongoDB\BSON\Document;
use MongoDB\BSON\VectorType;
use MongoDB\Driver\Exception\UnexpectedValueException;

require_once __DIR__ . '/../tests/utils/basic.inc';

$expectedFailures = [
    'Tests of Binary subtype 9, Vectors, with dtype FLOAT32: FLOAT32 with padding' => 'Document memory leak (PHPC-2648)',
    'Tests of Binary subtype 9, Vectors, with dtype FLOAT32: Insufficient vector data with 3 bytes FLOAT32' => 'Document memory leak (PHPC-2648)',
    'Tests of Binary subtype 9, Vectors, with dtype FLOAT32: Insufficient vector data with 5 bytes FLOAT32' => 'Document memory leak (PHPC-2648)',
    'Tests of Binary subtype 9, Vectors, with dtype INT8: INT8 with padding' => 'Document memory leak (PHPC-2648)',
    'Tests of Binary subtype 9, Vectors, with dtype PACKED_BIT: Padding specified with no vector data PACKED_BIT' => 'Document memory leak (PHPC-2648)',
    'Tests of Binary subtype 9, Vectors, with dtype PACKED_BIT: Exceeding maximum padding PACKED_BIT' => 'Document memory leak (PHPC-2648)',
];

$outputPath = realpath(__DIR__ . '/../tests') . '/bson-binary-vector/';

if ( ! is_dir($outputPath) && ! mkdir($outputPath, 0755, true)) {
    printf("Error creating output path: %s\n", $outputPath);
}

foreach (array_slice($argv, 1) as $inputFile) {
    if ( ! is_readable($inputFile) || ! is_file($inputFile)) {
        printf("Error reading %s\n", $inputFile);
        continue;
    }

    try {
        $test = Document::fromJSON(file_get_contents($inputFile))->toPHP(['root' => 'array', 'document' => 'array']);
    } catch (UnexpectedValueException $e) {
        printf("Error decoding %s: %s\n", $inputFile, $e->getMessage());
        continue;
    }

    if ( ! isset($test['description'])) {
        printf("Skipping test file without \"description\" field: %s\n", $inputFile);
        continue;
    }

    if ( ! isset($test['test_key'])) {
        printf("Skipping test file without \"test_key\" field: %s\n", $inputFile);
        continue;
    }

    if ( ! empty($test['tests'])) {
        foreach ($test['tests'] as $i => $case) {
            $outputFile = sprintf('%s-%03d.phpt', pathinfo($inputFile, PATHINFO_FILENAME), $i + 1);
            try {
                $output = renderPhpt(getParamsForTestCase($test, $case), $expectedFailures);
            } catch (Exception $e) {
                printf("Error processing tests[%d] in %s: %s\n", $i, $inputFile, $e->getMessage());
                continue;
            }

            if (false === file_put_contents($outputPath . '/' . $outputFile, $output)) {
                printf("Error writing tests[%d] in %s\n", $i, $inputFile);
                continue;
            }
        }
    }
}

function convertVector(array $input, VectorType $vectorType, int $padding): array
{
    if (!array_is_list($input)) {
        throw new InvalidArgumentException('Vector is not a list');
    }

    if ($vectorType != VectorType::PackedBit) {
        if ($padding !== 0) {
            throw new InvalidArgumentException(sprintf('Vector type %s does not support padding: %d', $vectorType->name, $padding));
        }

        return $input;
    }

    if ($padding < 0 || $padding > 7) {
        throw new InvalidArgumentException(sprintf('Expected padding [0..7], %d given', $padding));
    }

    if (count($input) === 0 && $padding > 0) {
        throw new InvalidArgumentException(sprintf('Non-zero padding (%d) given for empty vector', $padding));
    }

    $vector = [];

    foreach ($input as $i => $byte) {
        if (!is_int($byte)) {
            throw new InvalidArgumentException(sprintf('Expected vector[%d] to be an int, %s given', $i, get_debug_type($byte)));
        }

        if ($byte < 0 || $byte > 255) {
            throw new InvalidArgumentException(sprintf('Expected vector[%d] to be an unsigned int8, %d given', $i, $byte));
        }

        // decbin() may return fewer than 8 binary digits, so left-pad its output with zeroes
        $digits = str_pad(decbin($byte), 8, '0', STR_PAD_LEFT);

        $bits = array_map(intval(...), str_split($digits));

        array_push($vector, ...$bits);
    }

    // Remove trailing zeroes from the final byte's digits according to padding
    if ($padding > 0) {
        $removed = array_splice($vector, -$padding);

        // Assert that only zeroes were removed
        if ($removed !== array_fill(0, $padding, 0)) {
            throw new InvalidArgumentException(sprintf('Application of padding %d would remove non-zero digits: %s', $padding, json_encode($removed)));
        }
    }

    return $vector;
}

function getParamsForTestCase(array $test, array $case): array
{
    foreach (['description', 'valid', 'dtype_hex'] as $field) {
        if (!isset($case[$field])) {
            throw new InvalidArgumentException(sprintf('Missing "%s" field', $field));
        }
    }

    $code = '';
    $expect = '';

    $vectorType = match ($case['dtype_hex']) {
        '0x27' => VectorType::Float32,
        '0x03' => VectorType::Int8,
        '0x10' => VectorType::PackedBit,
    };

    $padding = $case['padding'] ?? 0;

    if ($case['valid']) {
        $code .= sprintf('// Vector %s with padding %d', json_encode($case['vector']), $padding) . "\n";

        /* encode a document from the numeric values, dtype, and padding, along
         * with the "test_key", and assert this matches the canonical_bson
         * string. */
        $vector = convertVector($case['vector'], $vectorType, $padding);
        $code .= sprintf('$vector = %s;', var_export($vector, true)) . "\n\n";
        $code .= sprintf('$binary = MongoDB\BSON\Binary::fromVector($vector, %s);', var_export($vectorType, true)) . "\n";
        $code .= sprintf('echo bin2hex((string) MongoDB\BSON\Document::fromPHP([%s => $binary])), "\n";', var_export($test['test_key'], true)) . "\n\n";
        $expect .= strtolower($case['canonical_bson']) . "\n";

        /* decode the canonical_bson into its binary form, and then assert that
         * the numeric values, dtype, and padding all match those provided in
         * the JSON. */
        $code .= sprintf('$bson = MongoDB\BSON\Document::fromBSON(hex2bin(%s));', var_export($case['canonical_bson'], true)) . "\n";
        $code .= sprintf('var_dump($binary == $bson[%s]);', var_export($test['test_key'], true)) . "\n";
        $expect .= 'bool(true)' . "\n";
    } else /* not valid */ {
        /* if the vector field is present, raise an exception when attempting
         * to encode a document from the numeric values, dtype, and padding. */
        if (isset($case['vector'])) {
            $code .= sprintf('// Vector %s with padding %d', json_encode($case['vector']), $padding) . "\n";

            try {
                $vector = convertVector($case['vector'], $vectorType, $padding);

                $code .= sprintf('$vector = %s;', var_export($vector, true)) . "\n\n";
                $code .= "throws(function() use (\$vector) {\n";
                $code .= sprintf("    var_dump(MongoDB\BSON\Binary::fromVector(\$vector, %s));\n", var_export($vectorType, true));
                $code .= "}, 'MongoDB\Driver\Exception\InvalidArgumentException');" . "\n\n";
                $expect .= 'OK: Got MongoDB\Driver\Exception\InvalidArgumentException' . "\n";
            } catch (InvalidArgumentException $e) {
                $code .= sprintf('echo %s, "\n";', var_export($e->getMessage(), true)) . "\n\n";
                $expect .= $e->getMessage() . "\n";
            }
        }

        /* if the canonical_bson field is present, raise an exception when
         * attempting to deserialize it into the corresponding numeric values,
         * as the field contains corrupted data. */
        if (isset($case['canonical_bson'])) {
            $code .= "throws(function() {\n";
            $code .= sprintf('    var_dump(MongoDB\BSON\Document::fromBSON(hex2bin(%s)));', var_export($case['canonical_bson'], true)) . "\n";
            $code .= "}, 'MongoDB\Driver\Exception\InvalidArgumentException');";
            $expect .= 'OK: Got MongoDB\Driver\Exception\InvalidArgumentException' . "\n";
        }
    }

    return [
        '%NAME%' => sprintf('%s: %s', trim($test['description']), trim($case['description'])),
        '%CODE%' => trim($code),
        '%EXPECT%' => trim($expect),
    ];
}

function renderPhpt(array $params, array $expectedFailures): string
{
     $params['%XFAIL%'] = isset($expectedFailures[$params['%NAME%']])
        ? "--XFAIL--\n" . $expectedFailures[$params['%NAME%']] . "\n"
        : '';

     $params['%SKIPIF%'] = '';

    $template = <<< 'TEMPLATE'
--TEST--
%NAME%
%XFAIL%%SKIPIF%--DESCRIPTION--
Generated by scripts/convert-bson-binary-vector-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

%CODE%

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
%EXPECT%
===DONE===
TEMPLATE;

    return str_replace(array_keys($params), array_values($params), $template);
}
