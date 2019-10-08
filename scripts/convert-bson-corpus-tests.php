<?php

require_once __DIR__ . '/../tests/utils/tools.php';

$expectedFailures = [
    'Double type: 1.23456789012345677E+18' => 'Variation in double\'s string representation (SPEC-850)',
    'Double type: -1.23456789012345677E+18' => 'Variation in double\'s string representation (SPEC-850)',
    'Int64 type: -1' => 'PHP encodes integers as 32-bit if range allows',
    'Int64 type: 0' => 'PHP encodes integers as 32-bit if range allows',
    'Int64 type: 1' => 'PHP encodes integers as 32-bit if range allows',
    'Javascript Code with Scope: Unicode and embedded null in code string, empty scope' => 'Embedded null in code string is not supported in libbson (CDRIVER-1879)',
    'Multiple types within the same document: All BSON types' => 'PHP encodes integers as 32-bit if range allows',
    'Top-level document validity: Bad $date (number, not string or hash)' => 'Legacy extended JSON $date syntax uses numbers (CDRIVER-2223)',
];

$for64bitOnly = [
    /* Note: Although 64-bit integers be represented by the Int64 class, these
     * tests fail on 32-bit platforms due to json_canonicalize() roundtripping
     * values through PHP, which converts large integers to floats. */
    'Int64 type: MinValue' => "Can't represent 64-bit ints on a 32-bit platform",
    'Int64 type: MaxValue' => "Can't represent 64-bit ints on a 32-bit platform",
];

$outputPath = realpath(__DIR__ . '/../tests') . '/bson-corpus/';

if ( ! is_dir($outputPath) && ! mkdir($outputPath, 0755, true)) {
    printf("Error creating output path: %s\n", $outputPath);
}

foreach (array_slice($argv, 1) as $inputFile) {
    if ( ! is_readable($inputFile) || ! is_file($inputFile)) {
        printf("Error reading %s\n", $inputFile);
        continue;
    }

    $test = json_decode(file_get_contents($inputFile), true);

    if (json_last_error() !== JSON_ERROR_NONE) {
        printf("Error decoding %s: %s\n", $inputFile, json_last_error_msg());
        continue;
    }

    if ( ! isset($test['description'])) {
        printf("Skipping test file without \"description\" field: %s\n", $inputFile);
        continue;
    }

    if ( ! empty($test['valid'])) {
        foreach ($test['valid'] as $i => $case) {
            $outputFile = sprintf('%s-valid-%03d.phpt', pathinfo($inputFile, PATHINFO_FILENAME), $i + 1);
            try {
                $output = renderPhpt(getParamsForValid($test, $case), $expectedFailures, $for64bitOnly);
            } catch (Exception $e) {
                printf("Error processing valid[%d] in %s: %s\n", $i, $inputFile, $e->getMessage());
                continue;
            }

            if (false === file_put_contents($outputPath . '/' . $outputFile, $output)) {
                printf("Error writing valid[%d] in %s\n", $i, $inputFile);
                continue;
            }
        }
    }

    if ( ! empty($test['decodeErrors'])) {
        foreach ($test['decodeErrors'] as $i => $case) {
            $outputFile = sprintf('%s-decodeError-%03d.phpt', pathinfo($inputFile, PATHINFO_FILENAME), $i + 1);
            try {
                $output = renderPhpt(getParamsForDecodeError($test, $case), $expectedFailures, $for64bitOnly);
            } catch (Exception $e) {
                printf("Error processing decodeErrors[%d] in %s: %s\n", $i, $inputFile, $e->getMessage());
                continue;
            }

            if (false === file_put_contents($outputPath . '/' . $outputFile, $output)) {
                printf("Error writing decodeErrors[%d] in %s\n", $i, $inputFile);
                continue;
            }
        }
    }

    if ( ! empty($test['parseErrors'])) {
        foreach ($test['parseErrors'] as $i => $case) {
            $outputFile = sprintf('%s-parseError-%03d.phpt', pathinfo($inputFile, PATHINFO_FILENAME), $i + 1);
            try {
                $output = renderPhpt(getParamsForParseError($test, $case), $expectedFailures, $for64bitOnly);
            } catch (Exception $e) {
                printf("Error processing parseErrors[%d] in %s: %s\n", $i, $inputFile, $e->getMessage());
                continue;
            }

            if (false === file_put_contents($outputPath . '/' . $outputFile, $output)) {
                printf("Error writing parseErrors[%d] in %s\n", $i, $inputFile);
                continue;
            }
        }
    }
}

function getParamsForValid(array $test, array $case)
{
    foreach (['description', 'canonical_bson', 'canonical_extjson'] as $field) {
        if (!isset($case[$field])) {
            throw new InvalidArgumentException(sprintf('Missing "%s" field', $field));
        }
    }

    $code = '';
    $expect = '';

    $lossy = isset($case['lossy']) ? (boolean) $case['lossy'] : false;

    $canonicalBson = $case['canonical_bson'];
    $expectedCanonicalBson = strtolower($canonicalBson);
    $code .= sprintf('$canonicalBson = hex2bin(%s);', var_export($canonicalBson, true)) . "\n";

    if (isset($case['degenerate_bson'])) {
        $degenerateBson = $case['degenerate_bson'];
        $expectedDegenerateBson = strtolower($degenerateBson);
        $code .= sprintf('$degenerateBson = hex2bin(%s);', var_export($degenerateBson, true)) . "\n";
    }

    if (isset($case['converted_bson'])) {
        $convertedBson = $case['converted_bson'];
        $expectedConvertedBson = strtolower($convertedBson);
        $code .= sprintf('$convertedBson = hex2bin(%s);', var_export($convertedBson, true)) . "\n";
    }

    $canonicalExtJson = $case['canonical_extjson'];
    $expectedCanonicalExtJson = json_canonicalize($canonicalExtJson);
    $code .= sprintf('$canonicalExtJson = %s;', var_export($canonicalExtJson, true)) . "\n";

    if (isset($case['relaxed_extjson'])) {
        $relaxedExtJson = $case['relaxed_extjson'];
        $expectedRelaxedExtJson = json_canonicalize($relaxedExtJson);
        $code .= sprintf('$relaxedExtJson = %s;', var_export($relaxedExtJson, true)) . "\n";
    }

    if (isset($case['degenerate_extjson'])) {
        $degenerateExtJson = $case['degenerate_extjson'];
        $expectedDegenerateExtJson = json_canonicalize($degenerateExtJson);
        $code .= sprintf('$degenerateExtJson = %s;', var_export($degenerateExtJson, true)) . "\n";
    }

    if (isset($case['converted_extjson'])) {
        $convertedExtJson = $case['converted_extjson'];
        $expectedConvertedExtJson = json_canonicalize($convertedExtJson);
        $code .= sprintf('$convertedExtJson = %s;', var_export($convertedExtJson, true)) . "\n";
    }

    $code .= "\n// Canonical BSON -> Native -> Canonical BSON \n";
    $code .= 'echo bin2hex(fromPHP(toPHP($canonicalBson))), "\n";' . "\n";
    $expect .= $expectedCanonicalBson . "\n";

    $code .= "\n// Canonical BSON -> Canonical extJSON \n";
    $code .= 'echo json_canonicalize(toCanonicalExtendedJSON($canonicalBson)), "\n";' . "\n";;
    $expect .= $expectedCanonicalExtJson . "\n";

    if (isset($relaxedExtJson)) {
        $code .= "\n// Canonical BSON -> Relaxed extJSON \n";
        $code .= 'echo json_canonicalize(toRelaxedExtendedJSON($canonicalBson)), "\n";' . "\n";;
        $expect .= $expectedRelaxedExtJson . "\n";
    }

    if (!$lossy) {
        $code .= "\n// Canonical extJSON -> Canonical BSON \n";
        $code .= 'echo bin2hex(fromJSON($canonicalExtJson)), "\n";' . "\n";
        $expect .= $expectedCanonicalBson . "\n";
    }

    if (isset($degenerateBson)) {
        $code .= "\n// Degenerate BSON -> Native -> Canonical BSON \n";
        $code .= 'echo bin2hex(fromPHP(toPHP($degenerateBson))), "\n";' . "\n";
        $expect .= $expectedCanonicalBson . "\n";

        $code .= "\n// Degenerate BSON -> Canonical extJSON \n";
        $code .= 'echo json_canonicalize(toCanonicalExtendedJSON($degenerateBson)), "\n";' . "\n";;
        $expect .= $expectedCanonicalExtJson . "\n";

        if (isset($relaxedExtJson)) {
            $code .= "\n// Degenerate BSON -> Relaxed extJSON \n";
            $code .= 'echo json_canonicalize(toRelaxedExtendedJSON($degenerateBson)), "\n";' . "\n";;
            $expect .= $expectedRelaxedExtJson . "\n";
        }
    }

    if (isset($degenerateExtJson) && !$lossy) {
        $code .= "\n// Degenerate extJSON -> Canonical BSON \n";
        $code .= 'echo bin2hex(fromJSON($degenerateExtJson)), "\n";' . "\n";
        $expect .= $expectedCanonicalBson . "\n";
    }

    if (isset($relaxedExtJson)) {
        $code .= "\n// Relaxed extJSON -> BSON -> Relaxed extJSON \n";
        $code .= 'echo json_canonicalize(toRelaxedExtendedJSON(fromJSON($relaxedExtJson))), "\n";' . "\n";
        $expect .= $expectedRelaxedExtJson . "\n";
    }

    return [
        '%NAME%' => sprintf('%s: %s', trim($test['description']), trim($case['description'])),
        '%CODE%' => trim($code),
        '%EXPECT%' => trim($expect),
    ];
}

function getParamsForDecodeError(array $test, array $case)
{
    foreach (['description', 'bson'] as $field) {
        if (!isset($case[$field])) {
            throw new InvalidArgumentException(sprintf('Missing "%s" field', $field));
        }
    }

    $code = sprintf('$bson = hex2bin(%s);', var_export($case['bson'], true)) . "\n\n";
    $code .= "throws(function() use (\$bson) {\n";
    $code .= "    var_dump(toPHP(\$bson));\n";
    $code .= "}, 'MongoDB\Driver\Exception\UnexpectedValueException');";

    /* We do not test for the exception message, since that may differ based on
     * the nature of the decoding error. */
    $expect = "OK: Got MongoDB\Driver\Exception\UnexpectedValueException";

    return [
        '%NAME%' => sprintf('%s: %s', trim($test['description']), trim($case['description'])),
        '%CODE%' => trim($code),
        '%EXPECT%' => trim($expect),
    ];
}

function getParamsForParseError(array $test, array $case)
{
    foreach (['description', 'string'] as $field) {
        if (!isset($case[$field])) {
            throw new InvalidArgumentException(sprintf('Missing "%s" field', $field));
        }
    }

    $code = '';
    $expect = '';

    switch ($test['bson_type']) {
        case '0x00': // Top-level document
            $code = "throws(function() {\n";
            $code .= sprintf("    fromJSON(%s);\n", var_export($case['string'], true));
            $code .= "}, 'MongoDB\Driver\Exception\UnexpectedValueException');";

            /* We do not test for the exception message, since that may differ
             * based on the nature of the parse error. */
            $expect = "OK: Got MongoDB\Driver\Exception\UnexpectedValueException";
            break;

        case '0x13': // Decimal128
            $code = "throws(function() {\n";
            $code .= sprintf("    new MongoDB\BSON\Decimal128(%s);\n", var_export($case['string'], true));
            $code .= "}, 'MongoDB\Driver\Exception\InvalidArgumentException');";

            /* We do not test for the exception message, since that may differ
             * based on the nature of the parse error. */
            $expect = "OK: Got MongoDB\Driver\Exception\InvalidArgumentException";
            break;

        default:
            throw new UnexpectedValueException(sprintf("Parse errors not supported for BSON type: %s", $test['bson_type']));
    }

    return [
        '%NAME%' => sprintf('%s: %s', trim($test['description']), trim($case['description'])),
        '%CODE%' => trim($code),
        '%EXPECT%' => trim($expect),
    ];
}

function renderPhpt(array $params, array $expectedFailures, array $for64bitOnly)
{
     $params['%XFAIL%'] = isset($expectedFailures[$params['%NAME%']])
        ? "--XFAIL--\n" . $expectedFailures[$params['%NAME%']] . "\n"
        : '';
     $params['%SKIPIF%'] = isset($for64bitOnly[$params['%NAME%']])
        ? "--SKIPIF--\n" . "<?php if (PHP_INT_SIZE !== 8) { die(\"skip {$for64bitOnly[$params['%NAME%']]}\"); } ?>" . "\n"
        : '';

    $template = <<< 'TEMPLATE'
--TEST--
%NAME%
%XFAIL%%SKIPIF%--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

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
